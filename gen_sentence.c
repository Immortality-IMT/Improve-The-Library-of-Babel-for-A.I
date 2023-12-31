/*
    Find the correct almanac
    Go to vegas, beat the casino
    Don't forget your Uncle Tony, send bitcoin: bc1qtgn8z30pcu0xrx2zgvcg9y6htr537mu4nd6qx3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define DICTIONARY_DATABASE "dictionary.db"
#define SENTENCES_DATABASE "sentences.db"

void concatenateAndStoreSentence(sqlite3 *db, const char *sentence) {
    // Output to standard output
    printf("Progress: Inserting into the database: %s\n", sentence);

    // Store the sentence in the SQLite database
    char sql[255];  // Adjust the size based on your needs
    snprintf(sql, sizeof(sql), "INSERT INTO sentences (sentence) VALUES ('%s');", sentence);

    if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
}

void generateAndStoreSentences(sqlite3 *db, char *words[], int length, int currentLength, char *currentSentence) {
    if (currentLength == 0) {
        // Store the generated sentence
        concatenateAndStoreSentence(db, currentSentence);
        return;
    }

    for (int i = 0; i < length; i++) {
        // Concatenate the current word to the sentence
        char *newSentence = malloc(strlen(currentSentence) + strlen(words[i]) + 2);
        snprintf(newSentence, strlen(currentSentence) + strlen(words[i]) + 2, "%s%s ", currentSentence, words[i]);

        // Generate sentences for the remaining length
        generateAndStoreSentences(db, words, length, currentLength - 1, newSentence);

        free(newSentence);
    }
}

int main() {
    sqlite3 *dictDb;
    if (sqlite3_open(DICTIONARY_DATABASE, &dictDb) != SQLITE_OK) {
        fprintf(stderr, "Error: Cannot open dictionary.db: %s\n", sqlite3_errmsg(dictDb));
        return 1;
    }

    // Assume you have a table named "dictionary" with a column named "word"
    char *errMsg = 0;
    if (sqlite3_exec(dictDb, "CREATE TABLE IF NOT EXISTS dictionary (id INTEGER PRIMARY KEY, word TEXT);", 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(dictDb);
        return 1;
    }

    // Retrieve words from the "dictionary" table
    char **words = NULL;
    char query[] = "SELECT word FROM dictionary;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(dictDb, query, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(dictDb));
        sqlite3_close(dictDb);
        return 1;
    }

    int wordIndex = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *word = (const char *)sqlite3_column_text(stmt, 0);
        words = realloc(words, (wordIndex + 1) * sizeof(char *));
        words[wordIndex] = strdup(word);
        printf("Debug: Word retrieved from dictionary.db: %s\n", words[wordIndex]);
        wordIndex++;
    }

    sqlite3_finalize(stmt);

    // Debug output before generating and storing sentences
    printf("Debug: Words retrieved from dictionary.db before generating and storing sentences:\n");
    for (int i = 0; i < wordIndex; i++) {
        printf("%s\n", words[i]);
    }

    // Open a connection to the sentences database
    sqlite3 *sentencesDb;
    if (sqlite3_open(SENTENCES_DATABASE, &sentencesDb) != SQLITE_OK) {
        fprintf(stderr, "Error: Cannot open sentences.db: %s\n", sqlite3_errmsg(sentencesDb));
        sqlite3_close(dictDb);
        return 1;
    }

    // Assume you have a table named "sentences" with a column named "sentence"
    if (sqlite3_exec(sentencesDb, "CREATE TABLE IF NOT EXISTS sentences (id INTEGER PRIMARY KEY, sentence TEXT);", 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(sentencesDb);
        sqlite3_close(dictDb);
        return 1;
    }

    // Generate and store sentences of varying lengths
    int maxSentenceLength;
    printf("Enter the maximum sentence length: ");
    scanf("%d", &maxSentenceLength);

    for (int sentenceLength = 2; sentenceLength <= maxSentenceLength; sentenceLength++) {
        printf("Generating and storing %d-word sentences:\n", sentenceLength);
        generateAndStoreSentences(sentencesDb, words, wordIndex, sentenceLength, "");
    }

    // Clean up
    for (int i = 0; i < wordIndex; i++) {
        free(words[i]);
    }

    // Close the database connections
    sqlite3_close(dictDb);
    sqlite3_close(sentencesDb);

    return 0;
}

