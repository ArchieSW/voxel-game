#pragma once
#include <cglm/cglm.h>
#include <stdbool.h>

#include "Camera.h"
#include "Shader.h"
#include "Primitives.h"
#include "LightObject.h"

#define MAX_SHADERS_COUNT 32

typedef struct {
    vec3 position;
    vec3 normal;
} Vertex;

typedef struct {
    Vertex* array;
    int size;
    int capacity;
} VertexArray;

typedef struct {
    unsigned* array;
    int size;
    int capacity;
} IndexArray;

typedef struct {
    Shader* shaders[MAX_SHADERS_COUNT];
    Camera* camera;
    // Primitives
    CubeArray* cubes;
    // Light
    LightObject* light_object;
} Renderer;

Renderer* init_renderer();
void delete_renderer(Renderer* renderer);

VertexArray* init_vertex_array(int n);
void realloc_vertex_array(VertexArray* va, int new_capacity);
void push_vertex(VertexArray* va, Vertex v);
void delete_vertex_array(VertexArray* va);