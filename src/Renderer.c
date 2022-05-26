#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glad/glad.h>


Renderer* init_renderer() {
    Renderer* renderer = malloc(sizeof(Renderer));
    renderer->shaders[0] = compile_shader("assets/model.vs", "assets/model.fs");
    if (!renderer->shaders[0]->successfully_compiled) {
        puts("ERROR::FAILED TO COMPILE SHADER\n");
    }
    renderer->shaders[1] = compile_shader("assets/model.vs", "assets/light.fs");
    if (!renderer->shaders[0]->successfully_compiled) {
        puts("ERROR::FAILED TO COMPILE LIGHTNING SHADER\n");
    }
    renderer->camera = init_camera((vec3){0.0f, 256.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f}, YAW, PITCH);
    renderer->cubes = create_cube_array(1);
    renderer->light_object = create_light_object((vec3){1.0f, 1.0f, 1.0f}, (vec3){0.0f, 6.0f, 0.0f});

    return renderer;
}

void delete_renderer(Renderer* renderer) {
    free(renderer);
}

VertexArray* init_vertex_array(int n) {
    VertexArray* va = malloc(sizeof(Vertex));
    va->array = calloc(n, sizeof(Vertex));
    va->capacity = n;
    va->size = n;
    return va;
}

void realloc_vertex_array(VertexArray* va, int new_capacity) {
    Vertex* new_array = calloc(new_capacity, sizeof(Vertex));
    for (int i = 0; i < va->size; ++i) {
        glm_vec3_copy(va->array[i].normal,new_array[i].normal);
        glm_vec3_copy(va->array[i].position, new_array[i].position);
    }
    free(va->array);
    va->array = new_array;
    va->capacity = new_capacity;
}

void push_vertex(VertexArray* va, Vertex v) {
    if (va->size + 1 > va->capacity) {
        realloc_vertex_array(va, va->capacity*2);
    }
    glm_vec3_copy(va->array[va->size].normal, v.normal);
    glm_vec3_copy(va->array[va->size++].position, v.position);
}

void delete_vertex_array(VertexArray* va) {
    free(va->array);
    free(va);
}