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
        //printf("muoviti");
        // do something every one second.
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

    draw_tetrimino(current_tetrimino, 255, 0, 0, 255);

    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_SPACE, pressed_spacebar, offset, 1);
    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_DOWN, pressed_down, pos_y, 20);
    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_RIGHT, pressed_right, pos_x, 20);
    DO_ON_BUTTON_DOWN(ctx, SDL_SCANCODE_LEFT, pressed_left, pos_x, -20);
}

int main(int argc, char** argv)
{
    context = sdl_context_new("Tetris", 300, 450, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    context->post_draw = draw;

    tetrimino_t* tetrimino = tetrimino_new(tetrimino_L, 2, context->width / 2 - 50, context->height / 2 - 50);
    current_tetrimino = tetrimino;

    sdl_context_update(context);

    return 0;
}