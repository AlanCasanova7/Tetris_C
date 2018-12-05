#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL.h>

#define TETRIMINO_SIZE      20
#define TETRIMINO_SEGMENT   25
#define TETRIMINO_MAP_SIZE  100

typedef struct tetrimino{
    char* tetrimino_map;
    int pos_x,pos_y;
    uint8_t offset;
}tetrimino_t;

tetrimino_t* tetrimino_new(char*, uint8_t, int, int);
void draw_tetrimino(tetrimino_t*, uint8_t, uint8_t, uint8_t, uint8_t);