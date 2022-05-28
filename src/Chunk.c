#include <stdlib.h>
#include "Chunk.h"
#include "Block.h"

Chunk* create_chunk() {
    Chunk* chunk = malloc(sizeof(Chunk));
    chunk->blocks = calloc(CHUNK_SIZE, sizeof(Block**));
    for (int i = 0; i < CHUNK_SIZE; ++i) {
        chunk->blocks[i] = calloc(CHUNK_SIZE, sizeof(Block*));
        for (int j = 0; j < CHUNK_SIZE; ++j) {
            chunk->blocks[i][j] = calloc(CHUNK_SIZE, sizeof(Block));
        }
    }
}

void delete_chunk(Chunk* chunk) {
    for (int i = 0; i < CHUNK_SIZE; ++i) {
        for (int j = 0; j < CHUNK_SIZE; ++j) {
            free(chunk->blocks[i][j]);
        }
        free(chunk->blocks[i]);
    }
    free(chunk->blocks);
    free(chunk);
}

// void create_chunk_mesh(Chunk* chunk) {
// }