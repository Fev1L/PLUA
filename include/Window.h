//
// Created by Fev1L on 07.04.2026.
//

#ifndef PLUA_WINDOW_H
#define PLUA_WINDOW_H
#include "SDL3/SDL_video.h"

class Window
{
public:
    bool Init();
    void PollEvents(bool& running);
    void BeginFrame();
    void EndFrame();
    void Shutdown();
private:
    SDL_Window* m_Window = nullptr;
};

#endif //PLUA_WINDOW_H