#pragma once
typedef enum {
    UT_INT,
    UT_BOOL,
    UT_FLOAT,
    UT_MAT4,
    UT_VEC3
} UniformType;

typedef struct {
    unsigned int GL_ID1, GL_ID2, ID;
    int successfully_compiled;
} Shader;

Shader* compile_shader(const char* shader1, const char* shader2);
void delete_shader(Shader* shader);
void use_shader(Shader* shader);
void set_uniform(Shader* shader, const char* uniform_name, UniformType type, void* data);