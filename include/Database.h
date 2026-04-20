//
// Created by Fev1L on 07.04.2026.
//

#ifndef PLUA_DATABASE_H
#define PLUA_DATABASE_H

#include <string>
#include <vector>
#include <sqlite3.h>

#include "Message.h"

class Database
{
public:
    bool Open(const std::string& path);
    void Close();

    bool InitTables();

    bool SaveMessage(const Message& message, int sessionId);
    std::vector<Message> LoadMessages(int sessionId);

private:
    sqlite3* m_DB = nullptr;
};

#endif //PLUA_DATABASE_H