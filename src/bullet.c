#include <stdlib.h>

#include "bullet.h"

struct Bullet *create_bullet(SDL_FRect *entity_box) {
    struct Bullet *b = malloc(sizeof(struct Bullet));
    b->rect.x = entity_box->x + (SHIP_SIZE / 2);
    b->rect.y = entity_box->y - (SHIP_SIZE * 0.05);
    b->velocity = SDL_randf() / 2;
    b->rect.h = 15;
    b->rect.w = 2;
    return b;
}

void destroy_bullet(size_t bullet_index, struct Bullet **bullet_buffer, int final_bullet_index) {
    free(bullet_buffer[bullet_index]);
    bullet_buffer[bullet_index] = bullet_buffer[final_bullet_index];
}