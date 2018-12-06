#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL.h>

#define TETRIMINO_SIZE              20
#define TETRIMINO_SEGMENT           25
#define TETRIMINO_MAP_SIZE          100
#define TETRIMINO_TILEMAP_LENGTH    330
#define MAP_COLUMNS                 15
#define TETRIMINO_COLUMNS           5
#define TETRIMINO_MAX_ARRAY_ROT     4

typedef struct color{
    uint8_t r, g, b, a;
}color_t;

typedef struct tetrimino{
    char* tetrimino_map;
    int pos_x,pos_y;
    uint8_t offset;
}tetrimino_t;

tetrimino_t* tetrimino_new(char*, uint8_t, int, int);
int get_map_index(int, int, char*);
void draw_tetrimino(tetrimino_t*, color_t*);
void draw_map(char*, color_t*);
void check_map_line_filled(char*);
void add_tetrimino_to_map(char*, tetrimino_t*);
void draw_tetrimino_on_ui(char* map, color_t* color, int px, int py);
