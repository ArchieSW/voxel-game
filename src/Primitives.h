#pragma once
#include "Shader.h"

#include <cglm/cglm.h>

typedef struct {
    vec3 position;
    vec3 color;
} Cube;

Cube create_cube(vec3 position, vec3 color);
void draw_cube(Cube cube, Shader* shader);
void delete_gl_data();

typedef struct {
    Cube* array;
    int size;
    int capacity;
} CubeArray;

CubeArray* create_cube_array(int n);
void realloc_cube_array(CubeArray* ca, int new_capacity);
void push_cube(CubeArray* ca, Cube new_cube);
void delete_cube_array(CubeArray* ca);
void draw_cubes(CubeArray* ca, Shader* shader);