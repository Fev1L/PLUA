//
// Created by Fev1L on 01.05.2026.
//

#ifndef PLUA_LLAMA_BACKEND_H
#define PLUA_LLAMA_BACKEND_H

#pragma once
#include <string>
#include "chat_history.h"
#include "config_loader.h"

bool InitLlama();
void RunLlama(ChatHistory& history, const CharacterConfig& character);
void ShutdownLlama();

#endif //PLUA_LLAMA_BACKEND_H