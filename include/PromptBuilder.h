//
// Created by Fev1L on 07.04.2026.
//

#ifndef PLUA_PROMPTBUILDER_H
#define PLUA_PROMPTBUILDER_H

#include <string>
#include <vector>
#include "CharacterProfile.h"
#include "Message.h"

class PromptBuilder
{
public:
    std::string BuildPrompt(
        const CharacterProfile& character,
        const std::vector<Message>& messages,
        const std::vector<std::string>& facts,
        const std::string& userMessage
    );
};

#endif //PLUA_PROMPTBUILDER_H