#define SDL_MAIN_HANDLED

#include <tetrimino.h>
#include <tetrimino_maps.h>
#include <context.h>

tetrimino_t* current_tetrimino = NULL;
sdl_context_t* context = NULL;
color_t* colors = NULL;

char pressed_spacebar = 0;
char pressed_right = 0;
char pressed_left = 0;
char pressed_down = 0;

void respawn_current_tetrimino(){
    current_tetrimino->pos_x = 0;
    current_tetrimino->pos_y = 0;
    current_tetrimino->offset= 0;
    current_tetrimino->tetrimino_map = tetrimino_T;
}

#define DO_ON_BUTTON_DOWN(context, key, key_pressed_char, variable_to_mod, value)\
    next_possible_pos = (tetrimino_t*)SDL_memcpy(next_possible_pos, current_tetrimino, sizeof(tetrimino_t));\
    if(get_key(context, key) && key_pressed_char){\
        next_possible_pos->variable_to_mod += value;\
        key_pressed_char = 0;\
    for (int i = next_possible_pos->offset * TETRIMINO_SEGMENT; i < (next_possible_pos->offset * TETRIMINO_SEGMENT) + TETRIMINO_SEGMENT; i++){\
        if(next_possible_pos->tetrimino_map[i] != 0){\
            int x = next_possible_pos->pos_x + (((i - (next_possible_pos->offset * TETRIMINO_SEGMENT)) % TETRIMINO_COLUMNS) * TETRIMINO_SIZE);\
            int y = next_possible_pos->pos_y + (((i - (next_possible_pos->offset * TETRIMINO_SEGMENT)) / TETRIMINO_COLUMNS) * TETRIMINO_SIZE);\
            int index = get_map_index(x, y, tilemap);\
            if(index != 0){\
                if(index < 9){\
                    add_tetrimino_to_map(tilemap, current_tetrimino);\
                    respawn_current_tetrimino();\
                }\
                return;\
            }\
            SDL_Log("x [%d] y [%d] index [%d]", x, y, index);\
        }\
    }\
    current_tetrimino->pos_y = next_possible_pos->pos_y;\
    current_tetrimino->pos_x = next_possible_pos->pos_x;\
    current_tetrimino->offset = next_possible_pos->offset;\
    } else if(!get_key(context, key))\
        key_pressed_char = 1;


float update_counter = 0;
void update(double delta_time){
    update_counter += delta_time;
    if(update_counter >= 1){
        SDL_Log("%f", delta_time);
        update_counter = 0;
    }
}

static void draw(sdl_context_t* ctx, double delta_time)
{
    update(delta_time);

    draw_tetrimino(current_tetrimino, colors);
    draw_map(tilemap, colors);

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

    colors = malloc(sizeof(color_t)*9);
    colors[0].r = 255;  colors[0].g = 0;    colors[0].b = 0;    colors[0].a = 255;
    colors[1].r = 255;  colors[1].g = 255;  colors[1].b = 0;    colors[1].a = 255;
    colors[2].r = 0;    colors[2].g = 255;  colors[2].b = 0;    colors[2].a = 255;
    colors[3].r = 0;    colors[3].g = 255;  colors[3].b = 200;  colors[3].a = 255;
    colors[4].r = 0;    colors[4].g = 160;  colors[4].b = 255;  colors[4].a = 255;
    colors[5].r = 0;    colors[5].g = 0;    colors[5].b = 255;  colors[5].a = 255;
    colors[6].r = 255;  colors[6].g = 0;    colors[6].b = 255;  colors[6].a = 255;
    colors[7].r = 255;  colors[7].g = 255;  colors[7].b = 255;  colors[7].a = 255;
    colors[8].r = 255;  colors[7].g = 255;  colors[7].b = 255;  colors[7].a = 255;

    sdl_context_update(context);

    return 0;
}