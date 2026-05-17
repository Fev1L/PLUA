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

std::string GetTodayFilename();

std::string GetTodayDate();

void CleanOldLogs();

bool SaveHistory(const ChatHistory& history, const std::string& filepath);
bool LoadHistory(ChatHistory& history, const std::string& filepath);

void LoadAllHistory(ChatHistory& history);

#endif //PLUA_CHAT_HISTORY_H