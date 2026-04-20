//
// Created by Fev1L on 07.04.2026.
//

#include "../include/SessionManager.h"

int SessionManager::CreateSession()
{
    m_CurrentSessionId++;
    return m_CurrentSessionId;
}

void SessionManager::SetCurrentSession(int sessionId)
{
    m_CurrentSessionId = sessionId;
}

int SessionManager::GetCurrentSession() const
{
    return m_CurrentSessionId;
}