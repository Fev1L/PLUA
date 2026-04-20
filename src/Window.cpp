//
// Created by Fev1L on 07.04.2026.
//

#include "../include/Window.h"
#include <iostream>

#include "SDL3/SDL_init.h"

bool Window::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL Init failed\n";
        return false;
    }

    m_Window = SDL_CreateWindow("AI Friend", 1280, 720, SDL_WINDOW_OPENGL);

    if (!m_Window)
    {
        std::cout << "Window creation failed\n";
        return false;
    }

    return true;
}

void Window::PollEvents(bool& running)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            running = false;
        }
    }
}

void Window::BeginFrame()
{
    //
}

void Window::EndFrame()
{
    //
}

void Window::Shutdown()
{
    if (m_Window)
    {
        SDL_DestroyWindow(m_Window);
    }

    SDL_Quit();
}