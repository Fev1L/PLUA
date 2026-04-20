//
// Created by Fev1L on 07.04.2026.
//

#include "../include/ModelRunner.h"

bool ModelRunner::Init(const std::string& modelPath)
{
    m_ModelPath = modelPath;
    return true;
}

std::string ModelRunner::GenerateResponse(const std::string& prompt)
{
    return "Model response to: " + prompt;
}