#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

sqlite3* open_database(const char* db_path);
void close_database(sqlite3* db);
int execute_query(sqlite3* db, const char* query);
int execute_query_with_callback(sqlite3* db, const char* query, 
                               int (*callback)(void*, int, char**, char**), 
                               void* data);
int execute_query_param(sqlite3* db, const char* query, const char* param);

#endif
