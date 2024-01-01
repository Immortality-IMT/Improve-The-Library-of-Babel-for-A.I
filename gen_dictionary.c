/*
    Find the correct almanac
    Go to vegas, beat the casino
    Don't forget your Uncle Tony, send bitcoin: bc1qtgn8z30pcu0xrx2zgvcg9y6htr537mu4nd6qx3

    Whatever is in the dictioanry is used in the generation of sentences.
    The character set should be as small as possible. 
    All information should be represented with the character set.
    All notations are in words and theasaurus can eliminate words. 
    At the user level, various parsors translate the output.

    The longest word is 45 letters, a sequential generaion of all possibilites of 26 (a..z) 
    to the length of 45 letters (pneumonoultramicroscopicsilicovolcanoconiosis).

    gcc -g -o babel_words gen_dictionary.c -lsqlite3

    1. Press any key to exit the program.
    2. Program resumes from the last generated word found in the database.
    3. We only need to generate the set once, upload your database to be added.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stdbool.h>

#define DATABASE_NAME "dictionary.db"
#define CHARACTER_SET "abcdefghijklmnopqrstuvwxyz"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Safely exit to the program by pressing any key
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

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif

bool isValidWord(const char *word) {
    // Implement custom validation logic here
    // Return true if the word is valid, false otherwise

    return true; // Placeholder, replace with your logic
}

bool isValidWordWithAI(const char *word) {
    //send the word to an A.I. to decide
    //if word should be in character set 

    return true; // Placeholder, replace with your logic
}

// Function to increment a string in the pattern described
void incrementString(char *str) {
    int i = strlen(str) - 1;
    while (i >= 0 && str[i] == 'z') {
        str[i] = 'a';
        i--;
    }
    if (i >= 0) {
        str[i]++;
    } else {
        // If all characters are 'z', add a new 'a' at the beginning
        memmove(str + 1, str, strlen(str) + 1);
        str[0] = 'a';
    }
}

// Function to insert a new record into the "dictionary" table
void insertIntoDictionary(sqlite3 *db, const char *word) {
    char sql[100];

  if (isValidWord(word) && isValidWordWithAI(word)) { //perform validation

    snprintf(sql, sizeof(sql), "INSERT INTO dictionary (word) VALUES ('%s');", word);

    char *errMsg = 0;
    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
  }
}


int main() {
    sqlite3 *db;
    int rc;

    // Open the SQLite database
    rc = sqlite3_open("dictionary.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    // Create a table if it doesn't exist
    char *errMsg = 0;
    if (sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS dictionary (id INTEGER PRIMARY KEY, word TEXT);", 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    char initialWord[46];

    // Prepare SQL query to get the last record from the "dictionary" table
    const char *sql = "SELECT word FROM dictionary ORDER BY id DESC LIMIT 1;";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    // Execute the query
    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        // If there is a record, get the initial word from the result
        strcpy(initialWord, (const char *)sqlite3_column_text(stmt, 0));
        incrementString(initialWord); // stop the initial word from being added to the database again.
    } else {
        // If no record exists, set the initial word to 'a'
        strcpy(initialWord, "a");
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Infinite loop to generate and print the sequence
    while (!_kbhit()) {
        printf("%s, ", initialWord);
        insertIntoDictionary(db, initialWord);
        incrementString(initialWord);
    }

    // Close the database
    sqlite3_close(db);

    return 0;
}
