#pragma once

#include <string>
#include <vector>
#include <sqlite3.h>

class SQLiteDB {
public:
    SQLiteDB(const std::string& dbPath);
    ~SQLiteDB();
    void execute(const std::string& sql);
private:
    sqlite3* db;
};
