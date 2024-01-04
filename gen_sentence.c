/*
    Find the correct almanac
    Go to Vegas, beat the casino
    Don't forget your Uncle Tony, send bitcoin: bc1qtgn8z30pcu0xrx2zgvcg9y6htr537mu4nd6qx3

    One only need to generate single sentences of ever-increasing lengths using a dictionary. 
    
    Punctuation is superficial, parsers add it for the user but never store it.
    
    Everything is in words energy equals mass multiplied by the speed of light squared can be
    parsed into e=mc2.
    
    Get the library up to processing by A.I. to determine truth without bias.

    gcc -g -o babel_sentences gen_sentence.c -lsqlite3

    Requires 3 databases
    which are copies of en_dictionary.db
    master file - en_dictionary.db
    transaction file - sentence.db
    results file - res_sentence.db 
  
    copy and paste en_dictionary.db and rename to produce all 3
    delete all the data in the res_sentence.db before running
    because the new results are placed there.

    to get 3 word sentences
    rename res_sentence after 2 word sentences are complete
    and delete sentence.db
    and delete all enties in an empty res_sentence.db

    prune the generation to e.g 1000 entries to keep sentence generation
    doable and directed. e.g for science, aardvark safe to exclude 

    parse the generation to A.I. to data mine.

    Resumes, does not start anew with each run.
    Important you must manually reset the config tabe in en_dictionary.db
    To start from the beginning again.
*/

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#define MAX_RESULT_LENGTH 1024

#define dictionary_DB "dictionary.db"  // small wordset dictioanry db
#define sentence_DB "sentence.db"      // swap results db with sentence db
#define result_DB "res_sentence.db"    // each time for longer sentences

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Safely exit the program by pressing any key
int _kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif

// Function to concatenate two words with a space in between
void concatenateWords(const char *word1, const char *word2, char *result) {
    strcpy(result, word1);       // Copy the first word
    strcat(result, " ");          // Concatenate a space
    strcat(result, word2);        // Concatenate the second word
}

// Keep the results database as short as possible by pruning
void prune() {

}

// Make sure a value exists in config table for resume from previous state
int initializeAndOpenDatabases(sqlite3 **db1, sqlite3 **db2, sqlite3 **dbRes) {
    // Open databases
    if (sqlite3_open(dictionary_DB, db1) != SQLITE_OK ||
        sqlite3_open(sentence_DB, db2) != SQLITE_OK ||
        sqlite3_open(result_DB, dbRes) != SQLITE_OK) {
        fprintf(stderr, "Cannot open databases\n");
        return 1;
    }

    // Initialize save state config in dictionary database
        char *errMsg;
        const char *sql = "CREATE TABLE IF NOT EXISTS config ("
                          "last_processed_entry_id_dict INTEGER, "
                          "last_processed_entry_id_sentence INTEGER);"
                          "INSERT OR REPLACE INTO config VALUES (1, 1);";

        if (sqlite3_exec(*db1, sql, 0, 0, &errMsg) != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", errMsg);
            sqlite3_free(errMsg);
            sqlite3_close(*db1);
            return 1;
        }

    return 0;
}

// Function to handle SQLite errors
void handleSQLiteError(const char *operation, sqlite3 *db, const char *query) {
    fprintf(stderr, "SQLite Error during %s: %s\n", operation, sqlite3_errmsg(db));
    if (query != NULL) {
        fprintf(stderr, "Query: %s\n", query);
    }

    sqlite3_close(db);
}

int readAndPrintValues(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *selectQuery = "SELECT last_processed_entry_id_dict, last_processed_entry_id_sentence FROM config;";
    if (sqlite3_prepare_v2(db, selectQuery, -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("Initial values: %d, %d\n",
                   sqlite3_column_int(stmt, 0),
                   sqlite3_column_int(stmt, 1));
        }
        sqlite3_finalize(stmt);
        return 0;
    } else {
        fprintf(stderr, "Error reading initial values from the database.\n");
        return 1;
    }
}

void updateConfigTable(sqlite3 *db, int *updatedDictId, int *updatedSentenceId) {
    char *errMsg;
    char query[150];

    sprintf(query, "UPDATE config SET last_processed_entry_id_dict = %d, last_processed_entry_id_sentence = %d;",
            *updatedDictId, *updatedSentenceId);

    if (sqlite3_exec(db, query, 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return;  // Return without updating values on error
    }

    sqlite3_stmt *stmt;
    const char *selectQuery = "SELECT last_processed_entry_id_dict, last_processed_entry_id_sentence FROM config;";
    if (sqlite3_prepare_v2(db, selectQuery, -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            *updatedDictId = sqlite3_column_int(stmt, 0);
            *updatedSentenceId = sqlite3_column_int(stmt, 1);
            printf("Updated values: %d, %d\n", *updatedDictId, *updatedSentenceId);
        }
        sqlite3_finalize(stmt);
    }
}

int main() {
    sqlite3 *db1, *db2, *dbRes;
    sqlite3_stmt *stmt1, *stmt2, *stmtRes, *stmtConfig, *stmtUpdateConfig;
    int dictionaryId = 1; // If no value is found in db program exits
    int sentenceId = 1;   // Both values keep track of resume on re-execution

    // Init the databases
    if (initializeAndOpenDatabases(&db1, &db2, &dbRes) != 0) {
        return 1; // Exit
    }
   
    // Prepare statements
    const char *query1 = "SELECT id, word FROM entries WHERE id > ?;";
    const char *query2 = "SELECT id, word FROM entries WHERE id > ?;";
    const char *insertQuery = "INSERT INTO entries (word, wordtype, definition) VALUES (?, 'Concatenated', ?)";

    // Select the last processed entry IDs from the config table in dictionary.db
    const char *selectConfigQuery = "SELECT last_processed_entry_id_dict, last_processed_entry_id_sentence FROM config";

    if (sqlite3_prepare_v2(db1, query1, -1, &stmt1, NULL) != SQLITE_OK ||
        sqlite3_prepare_v2(db2, query2, -1, &stmt2, NULL) != SQLITE_OK ||
        sqlite3_prepare_v2(dbRes, insertQuery, -1, &stmtRes, NULL) != SQLITE_OK ||
        sqlite3_prepare_v2(db1, selectConfigQuery, -1, &stmtConfig, NULL) != SQLITE_OK) {
        handleSQLiteError("preparing statements", dbRes, NULL);
        return 1;
    }

    // Check if there are last processed entry IDs in the config table in dictionary.db
    if (sqlite3_step(stmtConfig) == SQLITE_ROW) {
        dictionaryId = sqlite3_column_int(stmtConfig, 0);
        sentenceId = sqlite3_column_int(stmtConfig, 1);
        printf("Last processed Dictionary ID: %d\n", dictionaryId);
        printf("Last processed Sentence ID: %d\n", sentenceId);
    } else {
        printf("No values found in the config table in dictionary.db. Using default values.\n");
    }


   // Bind the value of dictionaryId to the query
    sqlite3_bind_int(stmt1, 1, dictionaryId);
    sqlite3_bind_int(stmt2, 1, sentenceId);


    // Iterate over the results, concatenate words, and save to res_sentence.db
    while (sqlite3_step(stmt1) == SQLITE_ROW) {
        dictionaryId = sqlite3_column_int(stmt1, 0);
        const char *word1 = (const char *)sqlite3_column_text(stmt1, 1);

        // Reset the second statement to the beginning for each word in the first list
        sqlite3_reset(stmt2);

        // Check for key press and exit if a key is pressed
        if (_kbhit()) {
            printf("Exiting the program...\n");
            goto cleanup;  // Use goto to perform cleanup before exiting
        }

        while (sqlite3_step(stmt2) == SQLITE_ROW) {
            sentenceId = sqlite3_column_int(stmt2, 0);
            const char *word2 = (const char *)sqlite3_column_text(stmt2, 1);

            char result[MAX_RESULT_LENGTH];
            concatenateWords(word1, word2, result);

            // Print the result to the screen
            printf("Concatenated Word: %s\n", result);

            // Insert the result into res_sentence.db
            sqlite3_bind_text(stmtRes, 1, result, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmtRes, 2, "Concatenated definition", -1, SQLITE_STATIC);

            if (sqlite3_step(stmtRes) != SQLITE_DONE) {
                handleSQLiteError("inserting into res_sentence.db", dbRes, insertQuery);
                goto cleanup;  // Use goto to perform cleanup before exiting
            }

            // Update the last processed entry IDs in the config table in dictionary.db
            updateConfigTable(db1, &dictionaryId, &sentenceId);

            // Print the current IDs
            printf("Dictionary ID: %d\n", dictionaryId);
            printf("Sentence ID: %d\n", sentenceId);

            // Check for key press and exit if a key is pressed
            if (_kbhit()) {
                printf("Exiting the program...\n");
                goto cleanup;  // Use goto to perform cleanup before exiting
            }

            sqlite3_reset(stmtRes);
        }
    }

cleanup:
    // Finalize statements and close databases
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_finalize(stmtRes);
    sqlite3_finalize(stmtConfig);
    sqlite3_close(db1);
    sqlite3_close(db2);
    sqlite3_close(dbRes);

    return 0;
}
