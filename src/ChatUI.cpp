//
// Created by Fev1L on 07.04.2026.
//

#include "../include/ChatUI.h"

void ChatUI::Render()
{
    ImGui::Begin("Chat");

    for (const auto& msg : m_Messages)
    {
        if (msg.role == MessageRole::User)
            ImGui::Text("You: %s", msg.text.c_str());
        else
            ImGui::Text("Friend: %s", msg.text.c_str());
    }

    ImGui::Separator();

    ImGui::InputText("##input", m_InputBuffer, sizeof(m_InputBuffer));

    if (ImGui::Button("Send"))
    {
        m_PendingMessage = m_InputBuffer;
        m_InputBuffer[0] = '\0';
    }

    ImGui::End();
}

bool ChatUI::HasPendingMessage() const
{
    return !m_PendingMessage.empty();
}

std::string ChatUI::ConsumePendingMessage()
{
    std::string msg = m_PendingMessage;
    m_PendingMessage.clear();
    return msg;
}

void ChatUI::AddMessage(const Message& message)
{
    m_Messages.push_back(message);
}

void ChatUI::SetGenerating(bool generating)
{
    m_IsGenerating = generating;
}