#include <stdio.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <string.h>
#include <cglm/cglm.h>

#include "Shader.h"

#define MAX_SHADER_SIZE 1024
#define MAX_STRING_SIZE 512
#define COMPILE_LOG_SIZE 512

Shader* compile_shader(const char* shader1, const char* shader2) {
    FILE* file = fopen(shader1, "r");
    if (!file) {
        fputs("[ERROR] FAILED TO OPEN SHADER FILE\n", stderr);
        return NULL;
    }

    char** file_input = calloc(MAX_SHADER_SIZE, sizeof(char*));
    for (int i = 0; i < MAX_SHADER_SIZE; ++i) {
        file_input[i] = calloc(MAX_STRING_SIZE, sizeof(char));
    }

    if (!file_input) {
        fputs("[ERROR] FAILED TO ALLOCATE MEMORY", stderr);
    }

    int size = 0;
    char* buffer = calloc(MAX_STRING_SIZE, sizeof(char));
    while (fgets(buffer, MAX_STRING_SIZE, file) != NULL) {
        strcpy(file_input[size++], buffer);
    }

    int n = strlen(shader1);
    GLenum shader_type = GL_VERTEX_SHADER;

    for (int i = n-2; i >= 0; --i) {
        if (shader1[i] == '.' && shader1[i+1] == 'f') {
            shader_type = GL_FRAGMENT_SHADER;
            break;
        }
    }

    Shader* shader = malloc(sizeof(Shader));
    shader->GL_ID1 = glCreateShader(shader_type);
    glShaderSource(shader->GL_ID1, size, (const char* const*)file_input, NULL);
    glCompileShader(shader->GL_ID1);

    glGetShaderiv(shader->GL_ID1, GL_COMPILE_STATUS, &shader->successfully_compiled);

    char compile_log_shader1[COMPILE_LOG_SIZE] = {0};
    if (!shader->successfully_compiled) {
        int log_size = 0;
        glGetShaderInfoLog(shader->GL_ID1, COMPILE_LOG_SIZE, &log_size, compile_log_shader1);
        compile_log_shader1[log_size] = '\0';
        fprintf(stderr, "[ERROR] FAILED TO COMPILE SHADER:\n[ERROR] COMPILER MESSAGE: %s", compile_log_shader1);
    } else {
        fputs("[INFO] SHADER SUCCESSFULLY COMPILED\n", stdout);
    }

    fclose(file);

    for (int i = 0; i < MAX_SHADER_SIZE; ++i) {
        free(file_input[i]);
    }
    free(file_input);
    free(buffer);

///////////////////////////// SECOND SHADER ///////////////////////////////////
    file = fopen(shader2, "r");
    if (!file) {
        fputs("[ERROR] FAILED TO OPEN SHADER ", stderr);
        return NULL;
    }

    file_input = calloc(MAX_SHADER_SIZE, sizeof(char*));
    for (int i = 0; i < MAX_SHADER_SIZE; ++i) {
        file_input[i] = calloc(MAX_STRING_SIZE, sizeof(char));
    }

    if (!file_input) {
        fputs("[ERROR] FAILED TO ALLOCATE MEMORY", stderr);
    }

    size = 0;
    buffer = calloc(MAX_STRING_SIZE, sizeof(char));
    while (fgets(buffer, MAX_STRING_SIZE, file) != NULL) {
        strcpy(file_input[size++], buffer);
    }

    n = strlen(shader2);
    shader_type = GL_VERTEX_SHADER;

    for (int i = n-2; i >= 0; --i) {
        if (shader2[i] == '.' && shader2[i+1] == 'f') {
            shader_type = GL_FRAGMENT_SHADER;
            break;
        }
    }

    shader->GL_ID2 = glCreateShader(shader_type);
    glShaderSource(shader->GL_ID2, size, (const char* const*)file_input, NULL);
    glCompileShader(shader->GL_ID2);

    glGetShaderiv(shader->GL_ID2, GL_COMPILE_STATUS, &shader->successfully_compiled);

    char compile_log_shader2[COMPILE_LOG_SIZE] = {0};
    if (!shader->successfully_compiled) {
        int log_size = 0;
        glGetShaderInfoLog(shader->GL_ID2, COMPILE_LOG_SIZE, &log_size, compile_log_shader2);
        compile_log_shader2[log_size] = '\0';
        printf("[ERROR] FAILED TO COMPILE SHADER:\n[ERROR] COMPILER MESSAGE: %s", compile_log_shader2);
    } else {
        fputs("[INFO] SHADER SUCCESSFULLY COMPILED\n", stdout);
    }


    fclose(file);

    shader->ID = glCreateProgram();
    glAttachShader(shader->ID, shader->GL_ID1);
    glAttachShader(shader->ID, shader->GL_ID2);
    glLinkProgram(shader->ID);

    glGetProgramiv(shader->ID, GL_LINK_STATUS, &shader->successfully_compiled);
    char link_log[COMPILE_LOG_SIZE];
    if (!shader->successfully_compiled) {
        glGetProgramInfoLog(shader->ID, COMPILE_LOG_SIZE, NULL, link_log);
        printf("[ERROR] FAILED TO LINK SHADER:\n[ERROR] COMPILER MESSAGE: %s", link_log);
    } else {
        fputs("[INFO] SHADERS SUCCESSFULLY LINKED\n", stdout);
    }

    
    glDeleteShader(shader->GL_ID1);
    glDeleteShader(shader->GL_ID2);

    for (int i = 0; i < MAX_SHADER_SIZE; ++i) {
        free(file_input[i]);
    }
    free(file_input);
    free(buffer);

    return shader;
}

void use_shader(Shader* shader) {
    glUseProgram(shader->ID);
}

void set_uniform(Shader* shader, const char* uniform_name, UniformType type, void* data) {
    switch (type)
    {
    case UT_FLOAT:
        glUniform1f(glGetUniformLocation(shader->ID, uniform_name), *((float*)data));
        break;
    case UT_INT:
    case UT_BOOL:
        glUniform1i(glGetUniformLocation(shader->ID, uniform_name), *((int*)data));
        break;
    case UT_MAT4:
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, uniform_name), 1, GL_FALSE, ((float*)data));
        break;
    case UT_VEC3:
        glUniform3f(glGetUniformLocation(shader->ID, uniform_name), ((float*)data)[0], ((float*)data)[1], ((float*)data)[2]);
        break;
    
    default:
        break;
    }
}

void delete_shader(Shader* shader) {
    glDeleteProgram(shader->ID);
    free(shader);
}