/*
    Find the correct almanac
    Go to vegas, beat the casino
    Don't forget your Uncle Tony, send bitcoin: bc1qtgn8z30pcu0xrx2zgvcg9y6htr537mu4nd6qx3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <curl/curl.h>

#define DATABASE_NAME "dictionary.db"
#define CHARACTER_SET "abcdefghijklmnopqrstuvwxyz"

#define OPENAI_API_KEY "YOUR_OPENAI_API_KEY"
#define OPENAI_API_ENDPOINT "https://api.openai.com/v1/engines/davinci/completions"

void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

// Function to perform HTTP POST request to OpenAI API
size_t curlWriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Handle the received data as needed (if necessary)
    return size * nmemb;
}

bool isValidWord(const char *word) {
    // Implement custom validation logic here
    // Return true if the word is valid, false otherwise

    return true; // Placeholder, replace with your logic
}

bool isValidWordWithGPT(const char *word) {
    CURL *curl;
    CURLcode res;

    // Initialize the curl session
    curl = curl_easy_init();
    if (curl) {
        // Construct the request payload
        char payload[512];
        snprintf(payload, sizeof(payload), "{\"prompt\": \"%s is a \", \"max_tokens\": 1}", word);

        // Set the API endpoint
        curl_easy_setopt(curl, CURLOPT_URL, OPENAI_API_ENDPOINT);

        // Set the request headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Bearer " OPENAI_API_KEY);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the request payload
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);

        // Set the write callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);

        // Perform the request
        res = curl_easy_perform(curl);

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        // Check if the request was successful
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            return false;  // Assume the word is not valid if there was an error
        }

        // Analyze the response and determine if the word is valid
        // (You need to implement this based on the OpenAI API response)

        // For simplicity, let's assume it's always valid for now
        return true;
    }

    return false;
}

void generatePermutationsAndStoreHelper(sqlite3 *db, const char *characters, int length, int current, char *currentWord) {
    if (current == length) {
        // Check if the word is valid before storing it
        if (isValidWord(currentWord) && isValidWordWithGPT(currentWord)) {
            printf("%s\n", currentWord); // Print the current word
            // Store the generated word in the SQLite database
            char *sql = malloc((strlen(currentWord) + 100) * sizeof(char));
            if (sql == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                sqlite3_close(db);
                exit(1);
            }

            snprintf(sql, strlen(currentWord) + 100, "INSERT INTO dictionary (word) VALUES ('%s');", currentWord);

            if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK) {
                fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            }

            free(sql);
        }

        return;
    }

    for (int i = 0; i < strlen(characters); i++) {
        currentWord[current] = characters[i];
        generatePermutationsAndStoreHelper(db, characters, length, current + 1, currentWord);
    }
}

void generateLanguagePermutationsAndStore(sqlite3 *db, const char *characters, int length) {
    char *currentWord = malloc((length + 1) * sizeof(char));
    if (currentWord == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        sqlite3_close(db);
        exit(1);
    }

    currentWord[length] = '\0';

    generatePermutationsAndStoreHelper(db, characters, length, 0, currentWord);

    free(currentWord);
}

int main() {

    char characters[] = CHARACTER_SET;
   
    // Delete the existing SQLite database file
    remove("dictionary.db");

    // Open a new connection to the SQLite database
    sqlite3 *db;
    if (sqlite3_open("dictionary.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Create a table if it doesn't exist
    char *errMsg = 0;
    if (sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS dictionary (id INTEGER PRIMARY KEY, word TEXT);", 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    printf("Enter the maximum word length (up to 45): "); //longest word in English is Pneumonoultramicroscopicsilicovolcanoconiosis
    int maxLength;
    scanf("%d", &maxLength);

    char *currentWord = malloc((maxLength + 1) * sizeof(char));
    if (currentWord == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        sqlite3_close(db);
        return 1;
    }

    for (int length = 1; length <= maxLength; length++) {
        currentWord[length] = '\0';

        printf("Permutations of words with length %d using '%s':\n", length, characters);
        generateLanguagePermutationsAndStore(db, characters, length);
    }

    // Close the SQLite database
    sqlite3_close(db);
    free(currentWord);

    return 0;
}

