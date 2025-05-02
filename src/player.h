#ifndef PLAYER_H
#define PLAYER_H

#include "bullet.h"
#include "circular_queue.h"
#include "quadtree.h"

#include <SDL3/SDL.h>

struct AppState;

struct ParticleTrail {
    SDL_FRect rect;
    int frame_count;
};

struct Player {
    int bullets_fired;
    char wasd;
    char lives;
    float velocity;
    int rotation;
    struct CircularQueue *cq;
    SDL_Texture *texture;
    SDL_FRect rect;
};

void initialize_player(struct Player *p, struct AppState *as,
                       SDL_Renderer *renderer);
void handle_input(SDL_Event *e, struct Player *p);
void update_player_movement(struct Player *p);
unsigned int update_bullets(struct Player *player, struct Bullet **bullets,
                            struct QTNode *q_tree, SDL_Renderer *renderer,
                            SDL_Texture *bullet_texture);
Uint32 fire_player_weapon(void *as, SDL_TimerID id, Uint32 interval);
void render_particle_trail(struct Player *p, SDL_Renderer *renderer);
#endif