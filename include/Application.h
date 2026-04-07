//
// Created by Fev1L on 07.04.2026.
//

#ifndef PLUA_APPLICATION_H
#define PLUA_APPLICATION_H

#include "Window.h"
#include "ModelRunner.h"
#include "Database.h"
#include "ChatUI.h"

class Application
{
public:
    bool Init();
    void Run();
    void Shutdown();

private:
    Window m_Window;
    ChatUI m_ChatUI;
    Database m_Database;
    ModelRunner m_ModelRunner;
};

#endif //PLUA_APPLICATION_H