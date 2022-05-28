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
    renderer->camera = init_camera((vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f}, YAW, PITCH);
    renderer->cubes = create_cube_array(1);
    renderer->light_object = create_light_object((vec3){1.0f, 1.0f, 1.0f}, (vec3){0.0f, 6.0f, 0.0f});

    return renderer;
}

void delete_renderer(Renderer* renderer) {
    free(renderer);
}

VertexArray* create_vertex_array(int n) {
    VertexArray* va = malloc(sizeof(Vertex));
    va->array = calloc(n, sizeof(Vertex));
    va->capacity = n;
    va->size = 0;
    return va;
}

void realloc_vertex_array(VertexArray* va, int new_capacity) {
    Vertex* new_array = calloc(new_capacity, sizeof(Vertex));
    for (int i = 0; i < va->size; ++i) {
        glm_vec3_copy(va->array[i].normal, new_array[i].normal);
        glm_vec3_copy(va->array[i].position, new_array[i].position);
        glm_vec3_copy(va->array[i].color, new_array[i].color);
    }
    free(va->array);
    va->array = new_array;
    va->capacity = new_capacity;
}

void push_vertex(VertexArray* va, Vertex v) {
    if (va->size + 1 > va->capacity) {
        realloc_vertex_array(va, va->capacity*2);
    }
    // printf("PRIKOL DA? %f %f %f AND %f %f %f NE ODNO I TOZHE AFTER COPY LOL\n", v.normal[0], v.normal[1], v.normal[2], va->array[va->size].normal[0], va->array[va->size].normal[1], va->array[va->size].normal[2]);
    glm_vec3_copy(v.normal, va->array[va->size].normal);
    glm_vec3_copy(v.position, va->array[va->size].position);
    glm_vec3_copy(v.color, va->array[va->size++].color);
}

void delete_vertex_array(VertexArray* va) {
    free(va->array);
    free(va);
}


IndexArray* create_index_array(int n) {
    IndexArray* ia = malloc(sizeof(IndexArray));
    ia->array = calloc(n, sizeof(unsigned));
    ia->capacity = n;
    ia->size = 0;
    return ia;
}

void realloc_index_array(IndexArray* ia, int new_capacity) {
    unsigned* new_array = calloc(new_capacity, sizeof(unsigned));
    for (int i = 0; i < ia->size; ++i) {
        new_array[i] = ia->array[i];
    }
    free(ia->array);
    ia->array = new_array;
    ia->capacity = new_capacity;
}

void push_index(IndexArray* ia, unsigned index) {
    if (ia->size + 1 < ia->capacity) {
        realloc_index_array(ia, ia->capacity * 2);
    }
    ia->array[ia->size++] = index;
}

void delete_index_array(IndexArray* ia) {
    free(ia->array);
    free(ia);
}

Mesh* create_mesh(VertexArray* vertices, IndexArray* indices) {
    Mesh* mesh = malloc(sizeof(Mesh));
    mesh->vertices = vertices;
    mesh->indices = indices;
    setup_mesh(mesh);
    return mesh;
}

void setup_mesh(Mesh* mesh) {
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices->size * sizeof(Vertex), mesh->vertices->array, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices->size * sizeof(unsigned), mesh->indices->array, GL_STATIC_DRAW);

    // Vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    // Vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // Vertex color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glBindVertexArray(0);
}

void draw_mesh(Mesh* mesh) {
    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->indices->size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void delete_mesh(Mesh* mesh) {
    delete_index_array(mesh->indices);
    delete_vertex_array(mesh->vertices);
    free(mesh);
}

void print_mesh_info(Mesh* mesh) {
    printf("INFO::MESH INFO:\n");
    printf("INFO::\tMESH VERTICES:\n");
    for (int i = 0; i < mesh->vertices->size; ++i) {
        printf("INFO::\t\tVertex #%d\n", i);
        printf("INFO::\t\t\tAttributes:\n");
        printf("INFO::\t\t\t\tPosition: %f %f %f\n", mesh->vertices->array[i].position[0], mesh->vertices->array[i].position[1], mesh->vertices->array[i].position[2]);
        printf("INFO::\t\t\t\tNormal: %f %f %f\n", mesh->vertices->array[i].normal[0], mesh->vertices->array[i].normal[1], mesh->vertices->array[i].normal[2]);
        printf("INFO::\t\t\t\tColor: %f %f %f\n", mesh->vertices->array[i].color[0], mesh->vertices->array[i].color[1], mesh->vertices->array[i].color[2]);
    }
    printf("INFO::\tMESH INDICES\n");
    printf("INFO::\t\t");
    for (int i = 0; i < mesh->indices->size; ++i) {
        printf("%d ", mesh->indices->array[i]);
    }
    printf("\n");
}