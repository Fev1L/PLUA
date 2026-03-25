#include <iostream>

#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "types.h"

//=================================================================
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]){
    App* app = new App();

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL Init error: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    app->window = SDL_CreateWindow("PLUA", 700, 700, SDL_WINDOW_OPENGL);
    app->renderer = SDL_CreateRenderer(app->window, nullptr);

    *appstate = app;
    return SDL_APP_CONTINUE;
}
//=================================================================
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{

    if (event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        SDL_Keycode key = event->key.key;
        if (key == SDLK_ESCAPE) return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}
//=================================================================
SDL_AppResult SDL_AppIterate(void* appstate)
{

    return SDL_APP_CONTINUE;
}
//=================================================================
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{

    SDL_Quit();
}
//=================================================================