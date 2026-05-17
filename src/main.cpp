#include <iostream>
#include <ostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <string>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

#include "../headers/llama_backend.h"
#include "../headers/ui.h"
#include "../headers/chat_history.h"
#include "../headers/config_loader.h"

int main() {
    EnsureConfigDir();

    CharacterConfig character;
    SettingsConfig  settings;
#ifdef __APPLE__
    std::string config_base = "config/";
#else
    std::string config_base = "config/";
#endif

    LoadCharacterConfig(character, ConfigPath("character.json"));
    LoadSettingsConfig(settings,   ConfigPath("settings.json"));

    if (!SDL_Init(SDL_INIT_VIDEO)) return -1;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow(
        character.name.c_str(),
        settings.window_width,
        settings.window_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl = SDL_GL_CreateContext(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForOpenGL(window, gl);
    ImGui_ImplOpenGL3_Init("#version 150");

    ApplyTheme(settings.theme);
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    InitLlama();
    CleanOldLogs();

    ChatHistory history;
    std::string current_date = GetTodayDate();
    LoadAllHistory(history);

    bool running = true;

    while (running) {
        int w, h;
        SDL_GetWindowSize(window, &w, &h);

        std::string today = GetTodayDate();
        if (today != current_date) {
            current_date = today;
            CleanOldLogs();
        }

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT) running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        std::string prompt;
        if (DrawUI(w, h, history, character, prompt)) {
            history.push_back({"user", prompt});
            RunLlama(history, character);
            SaveHistory(history, GetTodayFilename());
        }

        ImGui::Render();
        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ShutdownLlama();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(gl);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}