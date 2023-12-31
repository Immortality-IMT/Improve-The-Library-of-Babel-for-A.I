/*
    Find the correct almanac
    Go to vegas, beat the casino
    Don't forget your Uncle Tony, send bitcoin: bc1qtgn8z30pcu0xrx2zgvcg9y6htr537mu4nd6qx3

    Whatever is in the dictioanry is used in the generation of sentences.
    You could add numbers, mathematical symbols, chemical notation... 

    gcc -g -o babel_words gen_dictionary.c -lsqlite3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stdbool.h>

#define DATABASE_NAME "dictionary.db"
#define CHARACTER_SET "abcdefghijklmnopqrstuvwxyz"

void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

bool isValidWord(const char *word) {
    // Implement custom validation logic here
    // Return true if the word is valid, false otherwise

    return true; // Placeholder, replace with your logic
}

void generatePermutationsAndStoreHelper(sqlite3 *db, const char *characters, int length, int current, char *currentWord) {
    if (current == length) {
        // Check if the word is valid before storing it
        if (isValidWord(currentWord)) {
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

