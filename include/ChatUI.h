//
// Created by Fev1L on 07.04.2026.
//

#ifndef PLUA_CHATUI_H
#define PLUA_CHATUI_H

#include "string"
#include "vector"
#include "Message.h"

class ChatUI
{
public:
    void Render();

    bool HasPendingMessage() const;
    std::string ConsumePendingMessage();

    void AddMessage(const Message& message);

private:
    std::vector<Message> m_Messages;
};

#endif //PLUA_CHATUI_H