//
// Created by Fev1L on 07.04.2026.
//

#include "../include/PromptBuilder.h"

std::string PromptBuilder::BuildPrompt(
    const CharacterProfile& character,
    const std::vector<Message>& messages,
    const std::vector<std::string>& facts,
    const std::string& userMessage)
{
    std::string prompt;

    prompt += "Character:\n";
    prompt += character.GetName() + "\n\n";

    prompt += "Conversation:\n";

    for (const auto& msg : messages)
    {
        prompt += msg.text + "\n";
    }

    prompt += "\nUser: " + userMessage;

    return prompt;
}
