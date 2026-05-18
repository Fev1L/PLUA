//
// Created by Fev1L on 01.05.2026.
//

#ifndef PLUA_UI_H
#define PLUA_UI_H

#pragma once
#include <string>
#include "chat_history.h"
#include "config_loader.h"

void ApplyTheme(const std::string& theme);
void LoadAppFont(float size = 17.0f);

bool DrawUI(int window_w, int window_h,
            const ChatHistory& history,
            const CharacterConfig& character,
            std::string& prompt_out);

#endif //PLUA_UI_H