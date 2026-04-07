//
// Created by Fev1L on 07.04.2026.
//

#ifndef PLUA_MESSAGE_H
#define PLUA_MESSAGE_H

#include <string>

enum class MessageRole
{
    User,
    Assistant,
    System
};

struct Message
{
    MessageRole role;
    std::string text;
    std::string timestamp;
};

#endif //PLUA_MESSAGE_H