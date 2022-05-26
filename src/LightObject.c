#include "LightObject.h"
#include "Shader.h"
#include "Primitives.h"

extern unsigned cube_vao;
extern bool cube_initialized;

LightObject* create_light_object(vec3 color, vec3 position) {
    LightObject* lo = malloc(sizeof(LightObject));
    glm_vec3_copy(color, lo->color);
    glm_vec3_copy(position, lo->position);
    return lo;
}

void draw_light_object(LightObject* lo, Shader* shader) {
    if (cube_initialized) {
        Cube lo_cube = create_cube(lo->position, lo->color);
        draw_cube(lo_cube, shader);
    } else {
        printf("INFO::LIGHT OBJECT CAN NOT BE DRAWN CAUSE CUBE WAS NOT INITIALIZED\n");
    }
}

void delete_light_object(LightObject* lo) {
    free(lo);
}