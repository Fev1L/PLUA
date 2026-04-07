//
// Created by Fev1L on 07.04.2026.
//

#ifndef PLUA_SESSIONMANAGER_H
#define PLUA_SESSIONMANAGER_H

class SessionManager
{
public:
    int CreateSession();
    void SetCurrentSession(int sessionId);
    int GetCurrentSession() const;

private:
    int m_CurrentSessionId = 0;
};

#endif //PLUA_SESSIONMANAGER_H