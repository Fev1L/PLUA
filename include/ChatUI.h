//
// Created by Fev1L on 07.04.2026.
//

#ifndef PLUA_CHATUI_H
#define PLUA_CHATUI_H

#include "string"
#include "vector"
#include "Message.h"
#include <imgui.h>

class ChatUI
{
public:
    void Render();

    bool HasPendingMessage() const;
    std::string ConsumePendingMessage();

    void AddMessage(const Message& message);

    void SetGenerating(bool generating);

private:
    std::vector<Message> m_Messages;
    char m_InputBuffer[256] = "";
    std::string m_PendingMessage;
    bool m_IsGenerating = false;
};

#endif //PLUA_CHATUI_H