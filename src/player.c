#include "player.h"
#include "app_state.h"
#include "config.h"
#include "enemy.h"
#include "utils.h"
#include <math.h>

void initialize_player(struct Player *p, struct AppState *as,
                       SDL_Renderer *renderer) {
    p->rect.x = (SCREEN_WIDTH - SHIP_SIZE) / 2.0f;
    p->rect.y = (SCREEN_HEIGHT - SHIP_SIZE) * 0.85f;
    p->rect.w = SHIP_SIZE;
    p->rect.h = SHIP_SIZE;
    p->velocity = 0;
    p->rotation = 360;
    p->bullets_fired = 0;
    p->lives = PLAYER_DEFAULT_LIVES;

    p->texture = load_bmp_texture("assets/ship.bmp", renderer);
    if (!p->texture) {
        SDL_Log("Failed to create ship texture\n");
    }

    SDL_AddTimer(300, &fire_player_weapon, as);
}

void handle_input(SDL_Event *e, struct Player *p) {
    if (e->type == SDL_EVENT_KEY_DOWN) {
        switch (e->key.scancode) {
        case SDL_SCANCODE_W:
            p->wasd |= 1;
            break;
        case SDL_SCANCODE_A:
            p->wasd |= 2;
            break;
        case SDL_SCANCODE_S:
            p->wasd |= 4;
            break;
        case SDL_SCANCODE_D:
            p->wasd |= 8;
            break;
        case SDL_SCANCODE_SPACE:
            p->wasd |= 16;
            break;
        default:
            break;
        }
    }

    if (e->type == SDL_EVENT_KEY_UP) {
        switch (e->key.scancode) {
        case SDL_SCANCODE_W:
            p->wasd &= 30;
            break;
        case SDL_SCANCODE_A:
            p->wasd &= 29;
            break;
        case SDL_SCANCODE_S:
            p->wasd &= 27;
            break;
        case SDL_SCANCODE_D:
            p->wasd &= 23;
            break;
        case SDL_SCANCODE_SPACE:
            p->wasd &= 15;
            break;
        default:
            break;
        }
    }
}

void update_player_movement(struct Player *p) {
    double radian_rotation = p->rotation * (M_PI / 180);
    p->rect.y -= cos(radian_rotation) * p->velocity;
    p->rect.x += sin(radian_rotation) * p->velocity;
    if (p->wasd & 1) {
        if (p->velocity < PLAYER_SPEED) {
            p->velocity += 0.1;
        }
    } else {
        if (p->velocity > 0) {
            p->velocity -= 0.2;
        }
    }

    if (p->wasd & 2) {
        p->rotation -= PLAYER_SPEED;
        if (p->rotation <= 0) {
            p->rotation = 360;
        }
    }
    if (p->wasd & 8) {
        p->rotation += PLAYER_SPEED;
        if (p->rotation >= 360) {
            p->rotation = 0;
        }
    }
}

unsigned int update_bullets(struct Player *player, struct Bullet **bullets,
                            struct QTNode *q_tree, SDL_Renderer *renderer,
                            SDL_Texture *bullet_texture) {
    unsigned int death_count = 0;

    for (size_t i = 0; i < player->bullets_fired; ++i) {
        struct Bullet *b = bullets[i];
        double radian_rotation = b->rotation * (M_PI / 180);
        b->rect.y -= cos(radian_rotation) * 15;
        b->rect.x += sin(radian_rotation) * 15;

        struct Enemy *collided_enemy = qt_query(q_tree, &b->rect);

        // Draw hitboxes and decrement health
        if (collided_enemy != NULL) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderRect(renderer, &collided_enemy->rect);
            collided_enemy->health -= 35;
            if (collided_enemy->health <= 0) {
                death_count += 1;
            }
        }

        // De-allocate the bullets as they leave the screen
        if (b->rect.y < 0 || b->rect.y > SCREEN_HEIGHT || b->rect.x < 0 ||
            b->rect.x > SCREEN_WIDTH || collided_enemy != NULL) {
            destroy_bullet(i, bullets, --player->bullets_fired);
        }

        SDL_SetRenderDrawColor(renderer, 3, 215, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderTextureRotated(renderer, bullet_texture, NULL, &b->rect,
                                 b->rotation, NULL, SDL_FLIP_NONE);
    }
    return death_count;
}

Uint32 fire_player_weapon(void *as, SDL_TimerID id, Uint32 interval) {
    struct AppState *state = (struct AppState *)(as);

    if (state->paused) {
        return interval;
    }

    struct Player *p = &state->player;
    if (p->wasd & 16 && p->bullets_fired < PLAYER_NUM_BULLETS) {
        struct Bullet *b = create_bullet(&p->rect, p->rotation);
        state->bullets[++p->bullets_fired - 1] = b;
    }
    return interval;
}