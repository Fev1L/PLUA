//
// Created by Fev1L on 07.04.2026.
//

#include "../include/CharacterProfile.h"

bool CharacterProfile::LoadFromFile(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
        return false;

    nlohmann::json j;
    file >> j;

    m_Name = j["name"];
    m_Description = j["description"];

    return true;
}

const std::string& CharacterProfile::GetName() const
{
    return m_Name;
}