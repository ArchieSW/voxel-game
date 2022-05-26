#pragma once
#include <SDL2/SDL.h>

typedef struct {
    SDL_Window* window;    
    SDL_GLContext gl_context;
    int height;
    int width;
} Window;

Window* init_window(int width, int height, const char* title);
void delete_window(Window* window);