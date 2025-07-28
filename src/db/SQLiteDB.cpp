#include "SQLiteDB.h"

SQLiteDB::SQLiteDB(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Erorr opening database: " + std::string(sqlite3_errmsg(db)));
    }
}
SQLiteDB::~SQLiteDB() {
    sqlite3_close(db);
}
void SQLiteDB::execute(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error = "SQL error: " + std::string(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
}
