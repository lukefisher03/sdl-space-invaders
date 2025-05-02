#ifndef APP_STATE_H
#define APP_STATE_H

#include "bullet.h"
#include "levels/level.h"
#include "player.h"

struct AppState {
    int paused;
    struct Bullet *bullets[PLAYER_NUM_BULLETS];
    struct Bullet *enemy_bullets[ENEMY_BULLET_BUFFER_SIZE];
    SDL_Texture * bullet_texture;
    struct Player player;
    struct Level *active_level;
};

#endif