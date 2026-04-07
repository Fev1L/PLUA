//
// Created by Fev1L on 07.04.2026.
//

#ifndef PLUA_CHARACTERPROFILE_H
#define PLUA_CHARACTERPROFILE_H

#include "string"

class CharacterProfile
{
public:
    bool LoadFromFile(const std::string& path);

private:
    std::string m_Name;
    std::string m_Description;
};

#endif //PLUA_CHARACTERPROFILE_H