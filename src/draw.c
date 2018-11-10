#include "game.h"

//should merge the two values below

#define NB_TEXTURES 4

enum texture_id
{
    BACKGROUND = 0,
    MAP        = 1,
    PLAYER_RDR = 2,
    FOES       = 3
};

struct texture
{
    char *name;
    SDL_Rect rect;
};

struct texture textures[NB_TEXTURES] =
{
    { .name = "grass", .rect = {0, 0, BLOCK_SIZE,BLOCK_SIZE } },
    { .name = "player",.rect = { BLOCK_SIZE * 7, BLOCK_SIZE * 5,
                                 BLOCK_SIZE / 2, BLOCK_SIZE / 2 } },
    { .name = "finish", .rect = { BLOCK_SIZE * 3, BLOCK_SIZE * 4,
                                 BLOCK_SIZE,     BLOCK_SIZE }}
};

struct SDL_Rect get_sprite(const char *name)
{
    for (size_t i = 0; i < NB_TEXTURES; i++)
    {
        if (strcmp(name, textures[i].name) == 0)
        {
            return textures[i].rect;
        }
    }
    SDL_Rect null = {0, 0, 0, 0};
    return null;
}

const char* ressource_files[NB_TEXTURES] = 
{
    "resources/sprites/tiles.png",
    "resources/sprites/tiles.png",
    "resources/sprites/tiles.png",
    "resources/sprites/tiles.png",
};

void load_textures(struct gamestate *game)
{
    warnx("entered load_textures");
    game->texture_count = 0;
    //struct SDL_Window *window = game->window;
    struct list *textures;
    for (size_t i = 0; i < NB_TEXTURES; i++)
    {
        struct SDL_Renderer *renderer = game->renderer;
        SDL_Surface *blocks = IMG_Load(ressource_files[i]);
        if (!blocks)
            printf("could not load texture\n");
        SDL_Texture *blocks_texture=SDL_CreateTextureFromSurface(renderer,blocks); 
        if (i == 0)
            textures = list_init(blocks_texture);
        else
            list_add(textures, blocks_texture);
    }
    
    //struct SDL_Texture *textures =
    //    malloc(sizeof(struct SDL_Texture) * TEXTURE_MAX_AMOUNT);
   // if (!textures)
   //     return;
    
    game->textures = textures;
}

void init_window(struct gamestate *game)
{
    size_t height = game->map->height * BLOCK_SIZE;
    size_t width = game->map->width * BLOCK_SIZE;
   // size_t size = height * width;
    SDL_Window *window = SDL_CreateWindow("VGR_GAME",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height, 0);
    if (!window)
        return;
    printf("window created\n");
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, 0);
    game->window = window;
    game->renderer = renderer;
}

void render_map(struct gamestate *game)
{
    SDL_Renderer *renderer = game->renderer;
    SDL_Texture *texture = list_get_n(game->textures, MAP);
    struct map *map = game->map;

   // struct SDL_Rect *rect[map->width][map->height];
    // size_t size = game->map->width * game->map->height;
    map_print(map);
    for (size_t i = 0; i < map->width; i++)
    {
        for (size_t j = 0; j < map->height; j++)
        {
            enum block_type current = map_get_type(map, i, j);   
            struct SDL_Rect select = 
            {
                BLOCK_SIZE * i,
                BLOCK_SIZE * j,
                BLOCK_SIZE,
                BLOCK_SIZE
            };
            struct SDL_Rect sprite;
            if (current == BLOCK)
            {
                sprite = get_sprite("grass");
                warnx("found block in %zu/%zu\n", i, j);
            }
            else if (current == FINISH)
            {
                sprite = get_sprite("finish");
            }
            else
            {
                continue;
            }
            SDL_RenderCopy(renderer, texture, &sprite, &select);
        }
    }
}

void render_player(struct gamestate *game)
{
    SDL_Renderer *renderer = game->renderer;
    SDL_Texture *texture = game->textures->data;
    struct transform player_tf = game->player->transform;
    
    struct SDL_Rect player_position = 
    {
        BLOCK_SIZE * (player_tf.pos.x - player_tf.width / 2),
        BLOCK_SIZE * (player_tf.pos.y - player_tf.height / 2),
        BLOCK_SIZE * player_tf.width,
        BLOCK_SIZE * player_tf.height
    };
    warnx("player position : %f/%f\n",player_tf.pos.x, player_tf.pos.y);
    warnx("on image : %d / %d / %d / %d\n", player_position.x, player_position.y,player_position.w,player_position.h);

    struct SDL_Rect sprite = get_sprite("player");
    SDL_RenderCopy(renderer, texture, &sprite, &player_position); 
}


void render_game(struct gamestate *game)
{
    render_map(game);
    render_player(game);
    SDL_RenderPresent(game->renderer);
}

