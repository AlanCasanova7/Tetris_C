#include <context.h>

char* concat(const char* string, int value){
    char buffer[1024];
    memset(buffer, 0, 1024);
    sprintf_s(buffer, 1024, "%d", value);
    char* score = malloc(1024);
    memset(score, 0, 1024);
    int len = strlen(string);
    for(int i = 0; i < len; i++)
        score[i] = string[i];

    strcat_s(score, 1024, buffer);
    return score;
}

void draw_text_on_screen(sdl_context_t* context, char* text, int screen_x, int screen_y, int scale, uint8_t r, uint8_t g, uint8_t b){
    SDL_Color color = { r, g, b };
    SDL_Surface *text_surface;
    SDL_Texture* texture = NULL;
    SDL_Rect rect;
    rect.x = screen_x;
    rect.y = screen_y;

    text_surface = TTF_RenderText_Solid(context->font, text, color);

    rect.w = text_surface->w * scale;
    rect.h = text_surface->h * scale;
    if(text_surface)
    {
        texture = SDL_CreateTextureFromSurface(context->renderer, text_surface);
        SDL_FreeSurface(text_surface);
        if(!texture)
            return;

        SDL_RenderCopy(context->renderer, texture, NULL, &rect);
    }
    else
    {
        return;
    }
}

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

    if(!TTF_WasInit() && TTF_Init() == -1) 
     {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }

    context->font = TTF_OpenFont("sheeping.ttf", 16);

    if(!context->font) 
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
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
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double delta_time = 0;

    while(ctx->is_running)
    {
        SDL_PumpEvents();

        SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 0);
        SDL_RenderClear(ctx->renderer);

        if(get_key(ctx, SDL_SCANCODE_ESCAPE))
            ctx->is_running = 0;

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        delta_time = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
        delta_time *= 0.001;
        ctx->delta_time = delta_time;

        ctx->post_draw(ctx);

        SDL_RenderPresent(ctx->renderer);
    }
}