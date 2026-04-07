//
// Created by Fev1L on 07.04.2026.
//

#ifndef PLUA_DATABASE_H
#define PLUA_DATABASE_H

#include "Message.h"
#include "vector"

class Database
{
public:
    bool Open(const std::string& path);
    void Close();

    bool SaveMessage(const Message& message, int sessionId);
    std::vector<Message> LoadMessages(int sessionId);

private:
    //sqlite3* m_Database = nullptr;
};

#endif //PLUA_DATABASE_H