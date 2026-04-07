//
// Created by Fev1L on 07.04.2026.
//

#ifndef PLUA_MEMORYMANAGER_H
#define PLUA_MEMORYMANAGER_H

#include "string"
#include "vector"

class MemoryManager
{
public:
    void ProcessUserMessage(const std::string& message);
    std::vector<std::string> GetRelevantFacts(
        const std::string& userMessage
    );
};

#endif //PLUA_MEMORYMANAGER_H