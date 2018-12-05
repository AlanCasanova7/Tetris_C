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

float update_counter = 0;

void update(double delta_time){
    update_counter += delta_time;
    if(update_counter >= 1){
        current_tetrimino->pos_y += 20;
        update_counter = 0;
    }
}

#define DO_ON_BUTTON_DOWN(context, key, key_pressed_char, variable_to_mod, value) \
    if(get_key(context, key) && key_pressed_char){ \
        current_tetrimino->variable_to_mod += value; \
        key_pressed_char = 0; \
    } else if(!get_key(context, key)) \
        key_pressed_char = 1;

static void draw(sdl_context_t* ctx, double delta_time)
{
    update(delta_time);

    draw_tetrimino(current_tetrimino, colors);

    draw_map(tilemap, colors);

    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_SPACE, pressed_spacebar, offset, 1);
    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_DOWN, pressed_down, pos_y, 20);
    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_RIGHT, pressed_right, pos_x, 20);
    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_LEFT, pressed_left, pos_x, -20);
}

int main(int argc, char** argv)
{
    context = sdl_context_new("Tetris", 300, 440, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    context->post_draw = draw;

    tetrimino_t* tetrimino = tetrimino_new(tetrimino_L, 2, 20, 20);
    current_tetrimino = tetrimino;

    colors = malloc(sizeof(color_t)*8);
    colors[0].r = 255;  colors[0].g = 0;    colors[0].b = 0;    colors[0].a = 255;
    colors[1].r = 255;  colors[1].g = 255;  colors[1].b = 0;    colors[1].a = 255;
    colors[2].r = 0;    colors[2].g = 255;  colors[2].b = 0;    colors[2].a = 255;
    colors[3].r = 0;    colors[3].g = 255;  colors[3].b = 200;  colors[3].a = 255;
    colors[4].r = 0;    colors[4].g = 160;  colors[4].b = 255;  colors[4].a = 255;
    colors[5].r = 0;    colors[5].g = 0;    colors[5].b = 255;  colors[5].a = 255;
    colors[6].r = 255;  colors[6].g = 0;    colors[6].b = 255;  colors[6].a = 255;
    colors[7].r = 255;  colors[7].g = 255;  colors[7].b = 255;  colors[7].a = 255;

    sdl_context_update(context);

    return 0;
}