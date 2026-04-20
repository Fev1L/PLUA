//
// Created by Fev1L on 07.04.2026.
//

#include "../include/Database.h"
#include <iostream>

bool Database::Open(const std::string& path)
{
    if (sqlite3_open(path.c_str(), &m_DB) != SQLITE_OK)
    {
        std::cout << "Failed to open database\n";
        return false;
    }

    std::cout << "Database opened\n";
    return true;
}

void Database::Close()
{
    if (m_DB)
    {
        sqlite3_close(m_DB);
        m_DB = nullptr;
    }
}

bool Database::InitTables()
{
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS messages (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            session_id INTEGER,
            role TEXT,
            text TEXT,
            timestamp TEXT
        );
    )";

    char* errMsg = nullptr;

    if (sqlite3_exec(m_DB, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cout << "SQL error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }

    std::cout << "Tables initialized\n";
    return true;
}