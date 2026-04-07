//
// Created by Fev1L on 07.04.2026.
//

#ifndef PLUA_MODELRUNNER_H
#define PLUA_MODELRUNNER_H

#include <string>

class ModelRunner
{
public:
    bool Init(const std::string& modelPath);
    std::string GenerateResponse(const std::string& prompt);

private:
    std::string m_ModelPath;
};

#endif //PLUA_MODELRUNNER_H