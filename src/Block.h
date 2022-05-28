#pragma once
#include <stdbool.h>

#define BLOCK_SIZE 1.0f

typedef enum {
    BT_GRASS,
    BT_WATER
} BlockType;

typedef struct {
    bool is_active;
    BlockType type;
} Block;