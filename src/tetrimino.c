#include <tetrimino.h>
#include <context.h>

extern sdl_context_t *context;

char called;

tetrimino_t *tetrimino_new(char *map, uint8_t offset, int pos_x, int pos_y)
{
    tetrimino_t *tetrimino = malloc(sizeof(tetrimino_t));
    if (!tetrimino)
    {
        fprintf(stderr, "could not allocate space for tetrimino structure\n");
        return NULL;
    }
    memset(tetrimino, 0, sizeof(tetrimino_t));

    tetrimino->tetrimino_map = malloc(TETRIMINO_MAP_SIZE * sizeof(char));
    tetrimino->tetrimino_map = map;
    tetrimino->offset = offset;
    tetrimino->pos_x = pos_x;
    tetrimino->pos_y = pos_y;

    return tetrimino;
}

void draw_tetrimino(tetrimino_t *tetrimino, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_Rect rect;
    rect.w = TETRIMINO_SIZE;
    rect.h = TETRIMINO_SIZE;
    tetrimino->offset %= TETRIMINO_MAX_ARRAY_ROT;
    
    for (int i = tetrimino->offset * TETRIMINO_SEGMENT; i < (tetrimino->offset * TETRIMINO_SEGMENT) + TETRIMINO_SEGMENT; i++)
    {
        //SDL_Log("matrices points: [%d], [%d]", (((i - (tetrimino->offset * TETRIMINO_SEGMENT)) % TETRIMINO_COLUMNS) * rect.w) + tetrimino->pos_x, (((i - (tetrimino->offset * TETRIMINO_SEGMENT)) / TETRIMINO_COLUMNS) * rect.h) + tetrimino->pos_y);
        if (tetrimino->tetrimino_map[i] == 1)
        {
            rect.x = (((i - (tetrimino->offset * TETRIMINO_SEGMENT)) % TETRIMINO_COLUMNS) * rect.w) + tetrimino->pos_x;
            rect.y = (((i - (tetrimino->offset * TETRIMINO_SEGMENT)) / TETRIMINO_COLUMNS) * rect.h) + tetrimino->pos_y;

            SDL_SetRenderDrawColor(context->renderer, r, g, b, a);
            SDL_RenderFillRect(context->renderer, &rect);
            SDL_RenderDrawRect(context->renderer, &rect);
        }
    }
}

int get_map_index(int x, int y, char* map)
{
    int map_index = y * (MAP_COLUMNS) + x;
    return map[map_index / 20];
}

void draw_map(char *map)
{
    SDL_Rect rect;
    rect.w = TETRIMINO_SIZE;
    rect.h = TETRIMINO_SIZE;

    for (unsigned i = 0; i < TETRIMINO_TILEMAP_LENGTH; i++)
    {
        if (map[i] == 7)
        {
            rect.x = ((i % MAP_COLUMNS) * TETRIMINO_SIZE);
            rect.y = ((i / MAP_COLUMNS) * TETRIMINO_SIZE);

            SDL_SetRenderDrawColor(context->renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(context->renderer, &rect);
            SDL_RenderDrawRect(context->renderer, &rect);
        }
    }
}