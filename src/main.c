#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>

#include "app_state.h"
#include "bullet.h"
#include "config.h"
#include "enemy.h"
#include "levels/level.h"
#include "player.h"
#include "quadtree.h"
#include "star.h"
#include "utils.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

static struct Star stars[STAR_COUNT];

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    struct AppState *as = SDL_calloc(1, sizeof(struct AppState));
    struct Player *player = &as->player;

    if (!as) {
        return SDL_APP_FAILURE;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Sample SDL Project", SCREEN_WIDTH,
                                     SCREEN_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't initialize SDL window and renderer: %s\n",
                SDL_GetError());
        return SDL_APP_FAILURE;
    }

    initialize_player(player, as, renderer);
    initialize_stars(stars, STAR_COUNT);
    as->bullet_texture = load_bmp_texture("assets/bullet.bmp", renderer);
    if (!as->bullet_texture) {
        SDL_Log("Failed to create bullet texture\n");
    }

    as->active_level = build_level_1(as, renderer);
    *appstate = as;

    SDL_SetRenderVSync(renderer, 1);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {

    struct Player *player = &((struct AppState *)(appstate))->player;
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_WINDOW_FOCUS_LOST) {
        ((struct AppState *)appstate)->paused = 1;
        return SDL_APP_CONTINUE;
    }

    if (event->type == SDL_EVENT_WINDOW_FOCUS_GAINED) {
        ((struct AppState *)appstate)->paused = 0;
        return SDL_APP_CONTINUE;
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        ((struct AppState *)appstate)->paused =
            !((struct AppState *)appstate)->paused;
    }

    handle_input(event, player);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    struct AppState *as = (struct AppState *)(appstate);
    struct Player *player = &(as->player);
    struct QTNode *q_tree = calloc(1, sizeof(struct QTNode));

    qt_initialize(q_tree, SCREEN_WIDTH, SCREEN_HEIGHT);
    update_player_movement(player);

    as->active_level->q_tree = q_tree;

    if (as->paused) {
        SDL_Delay(500);
        return SDL_APP_CONTINUE;
    }

    SDL_SetRenderDrawColor(renderer, 10, 20, 30, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    render_level(as, renderer);

    unsigned int num_collisions = update_bullets(
        &as->player, as->bullets, q_tree, renderer, as->bullet_texture);
    as->active_level->live_enemy_count -= num_collisions;

    render_stars(stars, renderer, player);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderTextureRotated(renderer, player->texture, NULL, &player->rect,
                             player->rotation, NULL, SDL_FLIP_NONE);
    SDL_RenderDebugText(renderer, 20, 20, "GALAGA!");
    if (as->active_level->live_enemy_count <= 0) {
        SDL_RenderDebugText(renderer, 200, 20, "YOU WIN!");
    }
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 30);
    if (as->player.lives > 0) {
        for (size_t i = 0; i < as->player.lives; i++) {
            SDL_FRect r = {.h = SHIP_SIZE,
                           .w = SHIP_SIZE,
                           .x = SCREEN_WIDTH -
                                ((PLAYER_DEFAULT_LIVES * SHIP_SIZE) + 20) +
                                (SHIP_SIZE * i),
                           .y = 20};

            render_particle_trail(&as->player, renderer);
            SDL_RenderTexture(renderer, player->texture, NULL, &r);
        }
    } else {
        SDL_RenderDebugText(renderer, (SCREEN_WIDTH / 2) - 100, 300,
                            "YOU DIED!");
        as->paused = 1;
    }

    SDL_RenderPresent(renderer);

    qt_free(q_tree);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SDL_DestroyTexture(texture);
}