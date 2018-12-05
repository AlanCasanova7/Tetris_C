#include <context.h>

sdl_context_t* sdl_context_new(const char* title, int width, int height, uint32_t flags)
{
    sdl_context_t* context = malloc(sizeof(sdl_context_t));
    if(!context){
        fprintf(stderr, "could not allocate memory for context\n");
        return NULL;
    }

    memset(context, 0, sizeof(sdl_context_t));

    context->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if(!context->window){
        SDL_Log("could not creatw sdl window [%s]", SDL_GetError());
        return NULL;
    }

    context->renderer = SDL_CreateRenderer(context->window, -1, flags);
    if(!context->renderer){
        SDL_Log("could not creatw sdl renderer [%s]", SDL_GetError());
        return NULL;
    }

    context->key_state = SDL_GetKeyboardState(NULL);
    context->width = width;
    context->height = height;
    context->flags = flags;
    context->is_running = 1;

    return context;
}

uint8_t get_key(sdl_context_t* ctx, SDL_Scancode key)
{
    return ctx->key_state[key];
}

void sdl_context_update(sdl_context_t* ctx)
{
    while(ctx->is_running)
    {
        
        SDL_PumpEvents();

        SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 0);
        SDL_RenderClear(ctx->renderer);

        if(get_key(ctx, SDL_SCANCODE_ESCAPE))
            ctx->is_running = 0;

        ctx->post_draw(ctx);

        SDL_RenderPresent(ctx->renderer);
    }
}