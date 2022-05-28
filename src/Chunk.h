#pragma once
#include "Renderer.h"
#define CHUNK_SIZE 16

typedef struct {
    Block*** blocks;
    Mesh* mesh;
} Chunk;

Chunk* create_chunk();
void update_chunk();
void render_chunk();
void delete_chunk(Chunk* chunk);
void create_chunk_mesh(Chunk* chunk);