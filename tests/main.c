#define SDL_MAIN_HANDLED

#include <tetrimino.h>
#include <tetrimino_maps.h>
#include <context.h>

tetrimino_t* current_tetrimino = NULL;
sdl_context_t* context = NULL;

char pressed_spacebar = 0;
char pressed_right = 0;
char pressed_left = 0;
char pressed_down = 0;

float update_counter = 0;

void update(double delta_time){
    update_counter += delta_time;
    if(update_counter >= 1){
        SDL_Log("%f", delta_time);
        // do something every one second.
        //current_tetrimino->pos_y += 20;
        update_counter = 0;
    }
}

char once = 1;

#define DO_ON_BUTTON_DOWN(context, key, key_pressed_char, variable_to_mod, value)\
    next_possible_pos = (tetrimino_t*)SDL_memcpy(next_possible_pos, current_tetrimino, sizeof(tetrimino_t));\
    if(get_key(context, key) && key_pressed_char){\
        next_possible_pos->variable_to_mod += value;\
        key_pressed_char = 0;\
    for (int i = next_possible_pos->offset * TETRIMINO_SEGMENT; i < (next_possible_pos->offset * TETRIMINO_SEGMENT) + TETRIMINO_SEGMENT; i++){\
        if(next_possible_pos->tetrimino_map[i] == 1){\
            int x = next_possible_pos->pos_x + ((i % TETRIMINO_COLUMNS) * TETRIMINO_SIZE);\
            int y = next_possible_pos->pos_y + ((i / TETRIMINO_COLUMNS) * TETRIMINO_SIZE);\
            int index = get_map_index(x, y, tilemap);\
            if(index != 0){\
                return;\
            }\
            SDL_Log("x [%d] y [%d] index [%d]", x, y, index);\
        }\
    }\
    current_tetrimino->pos_y = next_possible_pos->pos_y;\
    current_tetrimino->pos_x = next_possible_pos->pos_x;\
    } else if(!get_key(context, key))\
        key_pressed_char = 1;


static void draw(sdl_context_t* ctx, double delta_time)
{
    update(delta_time);

    draw_tetrimino(current_tetrimino, 255, 0, 0, 255);

    draw_map(tilemap);
    tetrimino_t* next_possible_pos = malloc(sizeof(tetrimino_t));
    memset(next_possible_pos, 0, sizeof(tetrimino_t));
    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_SPACE, pressed_spacebar, offset, 1);
    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_DOWN, pressed_down, pos_y, 20);
    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_RIGHT, pressed_right, pos_x, 20);
    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_LEFT, pressed_left, pos_x, -20);
    free(next_possible_pos);
}

int main(int argc, char** argv)
{
    context = sdl_context_new("Tetris", 300, 440, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    context->post_draw = draw;

    tetrimino_t* tetrimino = tetrimino_new(tetrimino_L, 0, 20, 20);
    current_tetrimino = tetrimino;

    sdl_context_update(context);

    return 0;
}