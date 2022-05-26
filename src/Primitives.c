#include "Primitives.h"
#include "Renderer.h"

#include <glad/glad.h>
#include <stdbool.h>

unsigned cube_vao;
static unsigned cube_vbo;
bool cube_initialized = false;

static void load_cube_vao() {
    float cube_data[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned vao;
    unsigned vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_data), cube_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    cube_vao = vao;
    printf("INFO::CUBE INITIALIZED\n");
}

Cube create_cube(vec3 position, vec3 color) {
    if (!cube_initialized) {
        load_cube_vao();
        cube_initialized = true;
    }

    Cube cube;
    glm_vec3_copy(color, cube.color);
    glm_vec3_copy(position, cube.position);

    return cube;
}

void draw_cube(Cube cube, Shader* shader) {
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, cube.position);
    use_shader(shader);
    set_uniform(shader, "model", UT_MAT4, model);
    use_shader(shader);
    set_uniform(shader, "color", UT_VEC3, cube.color);
    glBindVertexArray(cube_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void delete_gl_data() {
    glDeleteBuffers(1, &cube_vbo);
    glDeleteVertexArrays(1, &cube_vao);
    printf("INFO::CUBE DATA DELETED FROM VRAM\n");
}

CubeArray* create_cube_array(int n) {
    CubeArray* ca = malloc(sizeof(CubeArray));
    ca->array = calloc(n, sizeof(Cube));
    ca->capacity = n;
    ca->size = 0;
    return ca;
}

void realloc_cube_array(CubeArray* ca, int new_capacity) {
    Cube* new_array = calloc(new_capacity, sizeof(Cube));
    for (int i = 0; i < ca->size; ++i) {
        glm_vec3_copy(ca->array[i].color, new_array[i].color);
        glm_vec3_copy(ca->array[i].position, new_array[i].position);
    }
    free(ca->array);
    ca->array = new_array;
    ca->capacity = new_capacity;
}

void push_cube(CubeArray* ca, Cube new_cube) {
    if (ca->size + 1 > ca->capacity) {
        realloc_cube_array(ca, ca->capacity*2);
    }
    glm_vec3_copy(new_cube.color, ca->array[ca->size].color);
    glm_vec3_copy(new_cube.position, ca->array[ca->size++].position);
}

void delete_cube_array(CubeArray* ca) {
    free(ca->array);
    free(ca);
    printf("INFO::CUBE ARRAY DELETED\n");
}

void draw_cubes(CubeArray* ca, Shader* shader) {
    for (int i = 0; i < ca->size; ++i) {
        draw_cube(ca->array[i], shader);
    }
}