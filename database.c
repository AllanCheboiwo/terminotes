#include "database.h"
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

sqlite3* open_database(const char* db_path) {

    if (db_path == NULL || (db_path[0] == '~' && db_path[1] == '/')) {
        const char* home = getenv("HOME");
        if (!home) {
            fprintf(stderr, "Error: Could not determine home directory\n");
            return NULL;
        }
        

        static char full_db_path[512];
        
        char config_dir[512];
        snprintf(config_dir, sizeof(config_dir), "%s/.config/terminotes", home);

        struct stat st = {0};
        if (stat(config_dir, &st) == -1) {

            if (mkdir(config_dir, 0755) == -1) {
                fprintf(stderr, "Error: Could not create config directory: %s\n", config_dir);
                return NULL;
            }
        }

        snprintf(full_db_path, sizeof(full_db_path), "%s/notes.db", config_dir);
        db_path = full_db_path;
    }
    
    sqlite3* db = NULL;
    int rc = sqlite3_open(db_path, &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    
    printf("Database opened successfully: %s\n", db_path);
    return db;
}

void close_database(sqlite3* db) {
    if (db) {
        sqlite3_close(db);
        printf("Database connection closed.\n");
    }
}

int execute_query(sqlite3* db, const char* query) {
    char* err_msg = NULL;
    int rc = sqlite3_exec(db, query, NULL, NULL, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    
    return 0;
}

int execute_query_with_callback(sqlite3* db, const char* query, 
                               int (*callback)(void*, int, char**, char**), 
                               void* data) {
    char* err_msg = NULL;
    int rc = sqlite3_exec(db, query, callback, data, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    
    return 0;
}


int execute_query_param(sqlite3* db, const char* query, const char* param) {
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    sqlite3_bind_text(stmt, 1, param, -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    
    sqlite3_finalize(stmt);
    return 0;
}
