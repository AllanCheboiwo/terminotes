#include "database.h"
#include <string.h>
#include <time.h>

#define DB_PATH "notes.db"

int print_results(void* data, int argc, char** argv, char** col_names);
void print_usage(const char* program_name);
int add_note(sqlite3* db, const char* note_text);
int read_notes(sqlite3* db, const char* timeframe);
const char* get_timeframe_condition(const char* timeframe);

int main(int argc, char *argv[]) {
    sqlite3* db = open_database(DB_PATH);
    if (!db) {
        return 1;
    }

    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS notes (" \
        "id INTEGER PRIMARY KEY AUTOINCREMENT," \
        "note_text TEXT NOT NULL," \
        "created_date DATETIME DEFAULT CURRENT_TIMESTAMP);";
    
    if (execute_query(db, create_table_sql) == 0) {
        printf("Notes table ready.\n");
    }

    if (argc < 2) {
        print_usage(argv[0]);
        close_database(db);
        return 1;
    }

    if (strcmp(argv[1], "take") == 0) {
        if (argc < 3) {
            printf("Error: Please provide a note text.\n");
            printf("Usage: %s take \"Your note here\"\n", argv[0]);
        } else {
            add_note(db, argv[2]);
        }
    }
    else if (strcmp(argv[1], "read") == 0) {
        const char* timeframe = "week";
        if (argc >= 3) {
            if (strcmp(argv[2], "-d") == 0) timeframe = "day";
            else if (strcmp(argv[2], "-w") == 0) timeframe = "week";
            else if (strcmp(argv[2], "-m") == 0) timeframe = "month";
            else if (strcmp(argv[2], "-a") == 0) timeframe = "all";
        }
        read_notes(db, timeframe);
    }
    else if (strcmp(argv[1], "help") == 0) {
        print_usage(argv[0]);
    }
    else {
        printf("Unknown command: %s\n", argv[1]);
        print_usage(argv[0]);
    }

    close_database(db);
    return 0;
}


int add_note(sqlite3* db, const char* note_text) {
    const char* query = "INSERT INTO notes (note_text) VALUES (?);";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    sqlite3_bind_text(stmt, 1, note_text, -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to insert note: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    
    sqlite3_finalize(stmt);
    printf("Note added successfully!\n");
    return 0;
}


int read_notes(sqlite3* db, const char* timeframe) {
    char query[1024];
    const char* condition = get_timeframe_condition(timeframe);
    
    snprintf(query, sizeof(query), 
             "SELECT id, note_text, created_date FROM notes %s ORDER BY created_date DESC;", 
             condition);
    
    printf("=== Your Notes (%s) ===\n", timeframe);
    execute_query_with_callback(db, query, print_results, NULL);
    return 0;
}


const char* get_timeframe_condition(const char* timeframe) {
    if (strcmp(timeframe, "day") == 0) {
        return "WHERE created_date >= datetime('now', '-1 day')";
    }
    else if (strcmp(timeframe, "week") == 0) {
        return "WHERE created_date >= datetime('now', '-7 days')";
    }
    else if (strcmp(timeframe, "month") == 0) {
        return "WHERE created_date >= datetime('now', '-30 days')";
    }
    else if (strcmp(timeframe, "all") == 0) {
        return ""; 
    }
    else {
        return "WHERE created_date >= datetime('now', '-7 days')";
    }
}

int print_results(void* data, int argc, char** argv, char** col_names) {
    if (argc >= 3) {
        printf("ID: %s\n", argv[0]);
        printf("Date: %s\n", argv[2]);
        printf("Note: %s\n", argv[1]);
        printf("---\n");
    }
    return 0;
}

void print_usage(const char* program_name) {
    printf("Notes App - Simple terminal note-taking\n\n");
    printf("Usage:\n");
    printf("  %s take \"Your note here\"    - Add a new note\n", program_name);
    printf("  %s read                   - Read notes from last week\n", program_name);
    printf("  %s read -d               - Read notes from last 24 hours\n", program_name);
    printf("  %s read -w               - Read notes from last week\n", program_name);
    printf("  %s read -m               - Read notes from last month\n", program_name);
    printf("  %s read -a               - Read all notes\n", program_name);
    printf("  %s help                  - Show this help\n", program_name);
    printf("\nExamples:\n");
    printf("  %s take \"Meeting with team at 3 PM\"\n", program_name);
    printf("  %s read -d\n", program_name);
}
