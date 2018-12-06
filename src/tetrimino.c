#include <tetrimino.h>
#include <context.h>

extern sdl_context_t *context;
extern int score;

#define MAX_LINE 13

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

void draw_tetrimino(tetrimino_t* tetrimino, color_t* color)
{
    SDL_Rect rect;
    rect.w = TETRIMINO_SIZE;
    rect.h = TETRIMINO_SIZE;
    tetrimino->offset %= TETRIMINO_MAX_ARRAY_ROT;

    for (int i = tetrimino->offset * TETRIMINO_SEGMENT; i < (tetrimino->offset * TETRIMINO_SEGMENT) + TETRIMINO_SEGMENT; i++)
    {
        if (tetrimino->tetrimino_map[i] != 0)
        {
            rect.x = (((i - (tetrimino->offset * TETRIMINO_SEGMENT)) % TETRIMINO_COLUMNS) * rect.w) + tetrimino->pos_x;
            rect.y = (((i - (tetrimino->offset * TETRIMINO_SEGMENT)) / TETRIMINO_COLUMNS) * rect.h) + tetrimino->pos_y;

            SDL_SetRenderDrawColor(context->renderer,
            color[(int)tetrimino->tetrimino_map[i] - 1].r,
            color[(int)tetrimino->tetrimino_map[i] - 1].g,
            color[(int)tetrimino->tetrimino_map[i] - 1].b,
            color[(int)tetrimino->tetrimino_map[i] - 1].a);
            //SDL_RenderFillRect(context->renderer, &rect);
            SDL_RenderDrawRect(context->renderer, &rect);
        }
    }
}

void destroyblocks(int line_num, char* map){
    score+=1;
    int start = (line_num - 1) * MAP_COLUMNS + 1;
    for(int i = start; i <= start + MAP_COLUMNS - 1; i++){
        int index = i;
        int index_up = i - MAP_COLUMNS;
        while(index_up > 0){
            map[index] = map[index_up];
            index = index_up;
            index_up -= 15;
        }
        // if(map[i - MAP_COLUMNS] != 0 && map[i - MAP_COLUMNS] < 8){
        //     map[i - MAP_COLUMNS] = 0;
        // }
    }
}

void check_map_line_filled(char* map)
{
    int line_num = 0;
    int number_of_tiles_completed = 0;
    for(int i = 0; i < TETRIMINO_TILEMAP_LENGTH; i++)
    {
        if(map[i] > 0 && map[i] < 8)
        {
            number_of_tiles_completed++;
        }

        if(number_of_tiles_completed == MAX_LINE)
        {
            number_of_tiles_completed = 0;
            destroyblocks(line_num, map);
            // map[i % 15 * 8]
        }

        if((i % MAP_COLUMNS) == 0)
        {
            number_of_tiles_completed = 0;
            line_num++;
        }
    }
}

int get_map_index(int x, int y, char* map)
{
    int map_index = y * (MAP_COLUMNS) + x;
    return map[map_index / 20];
}

void draw_map(char *map, color_t* color)
{
    SDL_Rect rect;
    rect.w = TETRIMINO_SIZE;
    rect.h = TETRIMINO_SIZE;

    for (unsigned i = 0; i < TETRIMINO_TILEMAP_LENGTH; i++)
    {
        if (map[i] != 0)
        {
            rect.x = ((i % MAP_COLUMNS) * TETRIMINO_SIZE);
            rect.y = ((i / MAP_COLUMNS) * TETRIMINO_SIZE);

            SDL_SetRenderDrawColor(context->renderer, color[(int)map[i] -1].r, color[(int)map[i] -1].g, color[(int)map[i] -1].b, color[(int)map[i] -1].a);
            SDL_RenderFillRect(context->renderer, &rect);
            SDL_RenderDrawRect(context->renderer, &rect);
        }
    }
}

void add_tetrimino_to_map(char *map, tetrimino_t* tetrimino)
{
    for (int i = tetrimino->offset * TETRIMINO_SEGMENT; i < (tetrimino->offset * TETRIMINO_SEGMENT) + TETRIMINO_SEGMENT; i++)
    {
        if(tetrimino->tetrimino_map[i] != 0)
        {
            int x = tetrimino->pos_x + (((i - (tetrimino->offset * TETRIMINO_SEGMENT)) % TETRIMINO_COLUMNS) * TETRIMINO_SIZE);
            int y = tetrimino->pos_y + (((i - (tetrimino->offset * TETRIMINO_SEGMENT)) / TETRIMINO_COLUMNS) * TETRIMINO_SIZE);
            SDL_Log("[%d], [%d]", x, y);
            int index = (y * MAP_COLUMNS + x) / 20;
            SDL_Log("[%d], [%d]", index, tetrimino->tetrimino_map[i]);
            map[index] = tetrimino->tetrimino_map[i];
        }
    }
}