#pragma once
#include "Window.h"
#include "Renderer.h"

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct {
    Window* window;
    bool is_running;
    Renderer* renderer;
    float delta_time;
} Application;

Application* create_application(int width, int height, const char* title);
void delete_application(Application* app);
void update(Application* app);
void render(Application* app);