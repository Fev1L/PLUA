//
// Created by Fev1L on 15.05.2026.
//

#ifndef PLUA_CONFIG_LOADER_H
#define PLUA_CONFIG_LOADER_H

#pragma once
#include <string>
#include <vector>

struct CharacterConfig {
    std::string name;
    std::string tone;
    std::string style;
    std::string description;
    std::vector<std::string> behavior_rules;
};

struct SettingsConfig {
    int   window_width  = 1280;
    int   window_height = 720;
    float font_size     = 16.0f;
    std::string theme   = "dark";
};
std::string ConfigPath(const std::string& filename);

void EnsureConfigDir();

bool LoadCharacterConfig(CharacterConfig& out, const std::string& filepath);
bool LoadSettingsConfig(SettingsConfig& out,   const std::string& filepath);

#endif //PLUA_CONFIG_LOADER_H