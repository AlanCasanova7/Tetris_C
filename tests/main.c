#define SDL_MAIN_HANDLED

#include <tetrimino.h>
#include <tetrimino_maps.h>
#include <context.h>

tetrimino_t* current_tetrimino = NULL;
sdl_context_t* context = NULL;

int pressed = 0;

static void draw(sdl_context_t* ctx)
{
    draw_tetrimino(current_tetrimino, 255, 0, 0, 255);

    if(get_key(context, SDL_SCANCODE_W))
        current_tetrimino->pos_y -= 20;
    if(get_key(context, SDL_SCANCODE_SPACE) && pressed){
        current_tetrimino->offset++;
        pressed = 0;
    } else if(!get_key(context, SDL_SCANCODE_SPACE))
        pressed = 1;

}

int main(int argc, char** argv)
{
    context = sdl_context_new("Tetris", 640, 480, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    context->post_draw = draw;

    tetrimino_t* tetrimino = tetrimino_new(tetrimino_L, 0, context->width / 2 - 50, context->height / 2 - 50);
    current_tetrimino = tetrimino;

    sdl_context_update(context);

    return 0;
}