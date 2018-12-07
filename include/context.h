#pragma once
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <text.h>

#include <SDL.h>

typedef struct sdl_context{
    SDL_Window* window;
    SDL_Renderer* renderer;
    const uint8_t* key_state;
    char is_running;
    int width,height;
    uint32_t flags;
    // char pressed;
    double delta_time;
    void(*post_draw)(struct sdl_context*);
    TTF_Font* font;
}sdl_context_t;

char* concat(const char*, int);
sdl_context_t* sdl_context_new(const char*, int, int, uint32_t);
uint8_t get_key(sdl_context_t*, SDL_Scancode);
void sdl_context_update(sdl_context_t*);
void draw_text_on_screen(sdl_context_t*, char*, int, int, int, uint8_t, uint8_t, uint8_t);
// uint8_t on_key_down(sdl_context_t*, SDL_Scancode);