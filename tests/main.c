#define SDL_MAIN_HANDLED

#include <tetrimino.h>
#include <tetrimino_maps.h>
#include <context.h>
#include <string.h>
#include <time.h>

tetrimino_t* current_tetrimino = NULL;
sdl_context_t* context = NULL;
color_t* colors = NULL;
tetrimino_t* next_possible_pos = NULL;

char pressed_spacebar = 0;
char pressed_right = 0;
char pressed_left = 0;
char pressed_down = 0;

float update_counter = 0;

char** map_modes = NULL;
char* next_map = NULL;
uint8_t next_offset = 0;

char* tetrimino_UI = NULL;

int score = 5;

#define MAX_STEP 20

void respawn_tetrimino(){
    if(current_tetrimino->pos_y < 0){
        SDL_Quit();
        context->is_running = 0;
        return;
    }

    current_tetrimino->offset = next_offset;
    current_tetrimino->pos_x = 80;
    current_tetrimino->pos_y = -20;
    current_tetrimino->tetrimino_map = next_map;
    next_map = map_modes[rand() % 7];
    tetrimino_UI = next_map;
}

#define DO_ON_BUTTON_DOWN(context, key, key_pressed_char, variable_to_mod, value)\
    next_possible_pos = (tetrimino_t*)SDL_memcpy(next_possible_pos, current_tetrimino, sizeof(tetrimino_t));\
    if(get_key(context, key) && key_pressed_char){\
        next_possible_pos->variable_to_mod += value;\
        next_possible_pos->offset %= 4;\
        key_pressed_char = 0;\
    for (int i = next_possible_pos->offset * TETRIMINO_SEGMENT; i < (next_possible_pos->offset * TETRIMINO_SEGMENT) + TETRIMINO_SEGMENT; i++){\
        if(next_possible_pos->tetrimino_map[i] != 0){\
            int x = next_possible_pos->pos_x + (((i - (next_possible_pos->offset * TETRIMINO_SEGMENT)) % TETRIMINO_COLUMNS) * TETRIMINO_SIZE);\
            int y = next_possible_pos->pos_y + (((i - (next_possible_pos->offset * TETRIMINO_SEGMENT)) / TETRIMINO_COLUMNS) * TETRIMINO_SIZE);\
            int index = get_map_index(x, y, tilemap);\
            if(index != 0){\
                if(index < 9){\
                    add_tetrimino_to_map(tilemap, current_tetrimino);\
                    respawn_tetrimino();\
                }\
                return;\
            }\
        }\
    }\
    current_tetrimino->pos_y = next_possible_pos->pos_y;\
    current_tetrimino->pos_x = next_possible_pos->pos_x;\
    current_tetrimino->offset = next_possible_pos->offset;\
    } else if(!get_key(context, key))\
        key_pressed_char = 1;

void update()
{
    next_possible_pos = (tetrimino_t*)SDL_memcpy(next_possible_pos, current_tetrimino, sizeof(tetrimino_t));
    update_counter += context->delta_time * (score / 5);
    if(update_counter >= 1)
    {
        next_possible_pos->pos_y += MAX_STEP;
        for (int i = next_possible_pos->offset * TETRIMINO_SEGMENT; i < (next_possible_pos->offset * TETRIMINO_SEGMENT) + TETRIMINO_SEGMENT; i++)
        {
            if(next_possible_pos->tetrimino_map[i] != 0)
            {
                int index = get_map_index(next_possible_pos->pos_x + (((i - (next_possible_pos->offset * TETRIMINO_SEGMENT)) % TETRIMINO_COLUMNS) * TETRIMINO_SIZE), next_possible_pos->pos_y + (((i - (next_possible_pos->offset * TETRIMINO_SEGMENT)) / TETRIMINO_COLUMNS) * TETRIMINO_SIZE), tilemap);
                if(index != 0){
                    if(index < 9){
                        add_tetrimino_to_map(tilemap, current_tetrimino);
                        respawn_tetrimino();
                    }
                    return;
                }
            }
        }
        current_tetrimino->pos_y = next_possible_pos->pos_y;
        current_tetrimino->pos_x = next_possible_pos->pos_x;

        update_counter = 0;
    }
}

static void draw(sdl_context_t* ctx)
{
    update();

    draw_tetrimino(current_tetrimino, colors);
    draw_map(tilemap, colors);

    draw_tetrimino_on_ui(tetrimino_UI, colors, ctx->width - 120, ctx->height - 120);

    check_map_line_filled(tilemap);

    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_SPACE, pressed_spacebar, offset, 1);
    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_DOWN, pressed_down, pos_y, MAX_STEP);
    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_RIGHT, pressed_right, pos_x, MAX_STEP);
    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_LEFT, pressed_left, pos_x, -MAX_STEP);
}

int main(int argc, char** argv)
{
    srand((unsigned)time(NULL));
    context = sdl_context_new("Tetris", 440, 440, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    context->post_draw = draw;

    map_modes = malloc(100 * 7);

    map_modes[0] = tetrimino_T;
    map_modes[1] = tetrimino_J;
    map_modes[2] = tetrimino_Z;
    map_modes[3] = tetrimino_O;
    map_modes[4] = tetrimino_S;
    map_modes[5] = tetrimino_L;
    map_modes[6] = tetrimino_I;

    next_possible_pos = malloc(sizeof(tetrimino_t));
    memset(next_possible_pos, 0, sizeof(tetrimino_t));

    tetrimino_t* tetrimino = tetrimino_new(map_modes[rand() % 7], 0, 80, -20);
    current_tetrimino = tetrimino;

    //calculate next random map and copy it onto UI
    next_map = map_modes[rand() % 7];
    tetrimino_UI = next_map;

    colors = malloc(sizeof(color_t)*9);
    colors[0].r = 255;  colors[0].g = 0;    colors[0].b = 0;    colors[0].a = 255;
    colors[1].r = 255;  colors[1].g = 255;  colors[1].b = 0;    colors[1].a = 255;
    colors[2].r = 0;    colors[2].g = 255;  colors[2].b = 0;    colors[2].a = 255;
    colors[3].r = 0;    colors[3].g = 255;  colors[3].b = 200;  colors[3].a = 255;
    colors[4].r = 0;    colors[4].g = 160;  colors[4].b = 255;  colors[4].a = 255;
    colors[5].r = 0;    colors[5].g = 0;    colors[5].b = 255;  colors[5].a = 255;
    colors[6].r = 255;  colors[6].g = 0;    colors[6].b = 255;  colors[6].a = 255;
    colors[7].r = 255;  colors[7].g = 255;  colors[7].b = 255;  colors[7].a = 255;
    //left side wall
    colors[8].r = 255;  colors[8].g = 255;  colors[8].b = 255;  colors[8].a = 255;

    sdl_context_update(context);

    return 0;
}