//
// Created by Fev1L on 01.05.2026.
//

#ifndef PLUA_CHAT_HISTORY_H
#define PLUA_CHAT_HISTORY_H

#pragma once
#include <string>
#include <vector>

struct Message {
    std::string role;
    std::string content;
};

using ChatHistory = std::vector<Message>;

#endif //PLUA_CHAT_HISTORY_H