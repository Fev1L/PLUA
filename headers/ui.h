//
// Created by Fev1L on 01.05.2026.
//

#ifndef PLUA_UI_H
#define PLUA_UI_H

#pragma once
#include <string>
#include "chat_history.h"

bool DrawUI(int window_w, int window_h,
            const ChatHistory& history,
            std::string& prompt_out);

#endif //PLUA_UI_H