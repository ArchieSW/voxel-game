#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <cglm/cglm.h>
#include <FastNoiseLite.h>
#include <time.h>

#include "Application.h"
#include "Renderer.h"
#include "Camera.h"
#include "Primitives.h"

static bool application_initialized = false;
static int init_application() {
    if (application_initialized)
        return 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "ERROR::INIT::FAILED TO INIT SDL");
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_LoadLibrary(NULL);
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        fprintf(stderr, "ERROR::INIT_APP::FAILED TO INITIALIZE GLAD\n");
        return 1;
    }
    glEnable(GL_DEPTH_TEST);
    fprintf(stdout, "INFO::OpenGL loaded\n");
    fprintf(stdout, "INFO::Vendor:\t%s\n", glGetString(GL_VENDOR));
    fprintf(stdout, "INFO::Renderer:\t%s\n", glGetString(GL_RENDERER));
    fprintf(stdout, "INFO::Version:\t%s\n", glGetString(GL_VERSION));

    SDL_SetRelativeMouseMode(SDL_TRUE);

    application_initialized = true;
    return 0;
}

Application* create_application(int width, int height, const char* title) {
    Application* app = malloc(sizeof(Application));
    app->delta_time = 0.0f;
    Window* window = init_window(width, height, title);
    init_application();
    app->window = window;
    app->is_running = true;

    #define SEED 3000

    fnl_state state = fnlCreateState();
    state.noise_type = FNL_NOISE_PERLIN;
    state.seed = SEED;

    #define LANDSCAPE_WIDTH 256
    #define LANDSCAPE_HEIGHT 256

    float* tmp = malloc(LANDSCAPE_WIDTH*LANDSCAPE_HEIGHT*sizeof(float));
    float* height_map[LANDSCAPE_HEIGHT];


    for (int y = 0; y < LANDSCAPE_HEIGHT; ++y) {
        for (int x = 0; x < LANDSCAPE_WIDTH; ++x) {
            height_map[x] = tmp + x*LANDSCAPE_HEIGHT;
        }
    }
    

    for (int y = 0; y < LANDSCAPE_HEIGHT; ++y) {
        for (int x = 0; x < LANDSCAPE_WIDTH; ++x) {
            height_map[y][x] = fnlGetNoise2D(&state, x, y);
        }
    }


    Renderer* renderer = init_renderer();

    Cube cube;

    #define WATER_LEVEL 64

    for (int x = -LANDSCAPE_WIDTH/2; x < LANDSCAPE_WIDTH/2; ++x) {
        for (int z = -LANDSCAPE_HEIGHT/2; z < LANDSCAPE_HEIGHT/2; ++z) {
            float y_coord = floor(255.0f*(height_map[x + LANDSCAPE_WIDTH/2][z + LANDSCAPE_WIDTH/2] + 1.0f)/4.0f);
            if (y_coord < WATER_LEVEL) {
                cube = create_cube((vec3){(float)x, WATER_LEVEL, (float)z}, (vec3){0.0f, 0.0f, 1.0f});
            } else {
                cube = create_cube((vec3){(float)x, y_coord, (float)z}, (vec3){0.0f, 1.0f, 0.0f});
            }
            push_cube(renderer->cubes, cube);
        }
    }
    free(tmp);

    app->renderer = renderer;

    return app;
}


int last_x = 1280/2;
int last_y = 1920/2;

void update(Application* app) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch (event.type)
        {
        case SDL_QUIT:
            app->is_running = false;
            break;
        case SDL_MOUSEMOTION: {
            int x_offset =  event.motion.xrel;
            int y_offset = -event.motion.yrel;

            // printf("%d %d\n", x_offset, y_offset);
            process_mouse_movement(app->renderer->camera, x_offset, y_offset, true);
            break;
        }

            break;

        default:
            break;
        }
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_W]) {
        process_keyboard(app->renderer->camera, CM_FORWARD, app->delta_time);
        // printf("INFO::W KEY PRESSED\n");
    } 
    if(state[SDL_SCANCODE_A]) {
        process_keyboard(app->renderer->camera, CM_LEFT, app->delta_time);
        // printf("INFO::A KEY PRESSED\n");
    } 
    if(state[SDL_SCANCODE_S]) {
        process_keyboard(app->renderer->camera, CM_BACKWARD, app->delta_time);
        // printf("INFO::S KEY PRESSED\n");
    } 
    if(state[SDL_SCANCODE_D]) {
        process_keyboard(app->renderer->camera, CM_RIGHT, app->delta_time);
        // printf("INFO::D KEY PRESSED\n");
    }
}

static void update_delta_time(Application* app) {
    static float last_time = 0.0f;
    float current_time = (float)SDL_GetTicks64() / 1000.0f;
    app->delta_time = current_time - last_time;
    last_time = current_time;
}

void render(Application* app) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    float current_time = (float)SDL_GetTicks64() / 1000.0f;

    update_delta_time(app);

    mat4 view = GLM_MAT4_IDENTITY_INIT;
    get_view_matrix(app->renderer->camera, view);

    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(glm_rad(45.0f), (float)app->window->width/(float)app->window->height, 0.1f, 1000.0f, projection);

    use_shader(app->renderer->shaders[0]);
    set_uniform(app->renderer->shaders[0], "view", UT_MAT4, view);
    set_uniform(app->renderer->shaders[0], "projection", UT_MAT4, projection);

    set_uniform(app->renderer->shaders[0], "light_color", UT_VEC3, app->renderer->light_object->color);
    set_uniform(app->renderer->shaders[0], "light_position", UT_VEC3, app->renderer->light_object->position);
    set_uniform(app->renderer->shaders[0], "camera_position", UT_VEC3, app->renderer->camera->position);

    draw_cubes(app->renderer->cubes, app->renderer->shaders[0]);

    use_shader(app->renderer->shaders[1]);
    set_uniform(app->renderer->shaders[1], "view", UT_MAT4, view);
    set_uniform(app->renderer->shaders[1], "projection", UT_MAT4, projection);
    set_uniform(app->renderer->shaders[1], "light_color", UT_VEC3, app->renderer->light_object->color);
    set_uniform(app->renderer->shaders[1], "light_position", UT_VEC3, app->renderer->light_object->position);
    set_uniform(app->renderer->shaders[1], "camera_position", UT_VEC3, app->renderer->camera->position);

    draw_light_object(app->renderer->light_object, app->renderer->shaders[1]);
    glm_vec3_copy((vec3){ cos(current_time/10)*60, sin(current_time/10)*60 + 100.0f, 0.0f }, app->renderer->light_object->position);


    SDL_GL_SwapWindow(app->window->window);
}

void delete_application(Application* app) {
    if(!app)
        return;
    delete_window(app->window);
    delete_shader(app->renderer->shaders[0]);
    delete_shader(app->renderer->shaders[1]);
    delete_camera(app->renderer->camera);
    delete_cube_array(app->renderer->cubes);
    delete_light_object(app->renderer->light_object);


    delete_renderer(app->renderer);
    delete_gl_data();
    free(app);
}