//
// Created by Fev1L on 07.04.2026.
//

#include "../include/Application.h"
#include <iostream>

bool Application::Init()
{
    if (!m_Window.Init())
    {
        return false;
    }

    if (!m_Database.Open("data/app.db"))
    {
        return false;
    }

    if (!m_Database.InitTables())
    {
        return false;
    }

    std::cout << "Application initialized\n";
    return true;
}

void Application::Run()
{
    while (m_IsRunning)
    {
        m_Window.PollEvents(m_IsRunning);

        m_Window.BeginFrame();

        m_ChatUI.Render();

        if (m_ChatUI.HasPendingMessage())
        {
            std::string userMsg = m_ChatUI.ConsumePendingMessage();

            Message msg;
            msg.role = MessageRole::User;
            msg.text = userMsg;

            m_ChatUI.AddMessage(msg);

            Message reply;
            reply.role = MessageRole::Assistant;
            reply.text = "I heard you: " + userMsg;

            m_ChatUI.AddMessage(reply);
        }

        m_Window.EndFrame();
    }
}

void Application::Shutdown()
{
    m_Database.Close();
    m_Window.Shutdown();
}