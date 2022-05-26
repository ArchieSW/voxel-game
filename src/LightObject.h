#pragma once
#include <cglm/cglm.h>
#include "Shader.h"

typedef struct {
    vec3 color;
    vec3 position;
} LightObject;

LightObject* create_light_object(vec3 color, vec3 position);
void draw_light_object(LightObject* lo, Shader* shader);
void delete_light_object(LightObject* lo);