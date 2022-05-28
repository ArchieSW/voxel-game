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
    vec3 color;
} Vertex;

typedef struct {
    Vertex* array;
    int size;
    int capacity;
} VertexArray;

VertexArray* create_vertex_array(int n);
void realloc_vertex_array(VertexArray* va, int new_capacity);
void push_vertex(VertexArray* va, Vertex v);
void delete_vertex_array(VertexArray* va);

typedef struct {
    unsigned* array;
    int size;
    int capacity;
} IndexArray;

IndexArray* create_index_array(int n);
void realloc_index_array(IndexArray* ia, int new_capacity); void push_index(IndexArray* ia, unsigned index);
void delete_index_array(IndexArray* ia);

typedef struct {
    VertexArray* vertices;
    IndexArray* indices;
    unsigned VAO;
    unsigned VBO;
    unsigned EBO;
} Mesh;

Mesh* create_mesh(VertexArray* vertices, IndexArray* indices);
void delete_mesh(Mesh* mesh);
void draw_mesh(Mesh* mesh);
void setup_mesh(Mesh* mesh);
void print_mesh_info(Mesh* mesh);

typedef struct {
    Shader* shaders[MAX_SHADERS_COUNT];
    Camera* camera;
    // Primitives
    CubeArray* cubes;
    // Light
    LightObject* light_object;
    Mesh* mesh;
} Renderer;

Renderer* init_renderer();
void delete_renderer(Renderer* renderer);