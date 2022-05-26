#include "Window.h"

Window* init_window(int width, int height, const char* title) {
    Window* new_window = malloc(sizeof(Window));
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (!window) {
        fprintf(stderr, "ERROR::WINDOW::FAILED TO CREATE WINDOW");
        return NULL;
    }
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        fprintf(stderr, "ERROR::WINDOW::FAILED TO CREATE GL CONTEXT");
        return NULL;
    }
    new_window->gl_context = context;
    new_window->window = window;
    new_window->height = height;
    new_window->width = width;
    return new_window;
}

void delete_window(Window* window) {
    if (!window)
        return;
    SDL_DestroyWindow(window->window);
    free(window);
}