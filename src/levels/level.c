#include "level.h"
#include "../app_state.h"
#include "../config.h"

int determine_cluster_direction(struct EnemyCluster *cluster);

void render_level(struct AppState *as, SDL_Renderer *renderer) {
    struct Level *level = as->active_level;
    if (!level || !renderer) {
        printf("Failed to render level");
        return;
    }

    for (size_t i = 0; i < level->num_clusters; ++i) {
        struct EnemyCluster *cluster = &level->enemy_clusters[i];
        cluster->direction *= determine_cluster_direction(cluster);
        if (cluster->size != 0) {
            for (size_t j = 0; j < cluster->size; ++j) {
                struct Enemy *e = &cluster->enemies[j];

                e->rect.x += cluster->direction * ENEMY_SPEED;

                if (e->health > 0) {
                    qt_add_node(level->q_tree, e);
                    SDL_RenderTexture(renderer, cluster->texture, NULL,
                                      &e->rect);
                }
            }
        }
    }

    for (size_t i = 0; i < level->enemy_bullets_fired; ++i) {
        struct Bullet *b = as->enemy_bullets[i];
        b->rect.y += BULLET_SPEED;
        SDL_SetRenderDrawColor(renderer, 215, 100, 100, SDL_ALPHA_OPAQUE);
        SDL_RenderTexture(renderer, as->bullet_texture, NULL, &b->rect);

        if (SDL_HasRectIntersectionFloat(&b->rect, &as->player.rect)) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderRect(renderer, &as->player.rect);
            as->player.lives--;
            free(as->enemy_bullets[i]);
            as->enemy_bullets[i] = as->enemy_bullets[--level->enemy_bullets_fired];
        }
    }
}

int determine_cluster_direction(struct EnemyCluster *cluster) {
    if (cluster->shift_distance) {
        if (cluster->enemies[0].rect.x >
            cluster->box.x + cluster->shift_distance) {
            return -1;
        } else if (cluster->enemies[0].rect.x < cluster->box.x) {
            return -1;
        }
    }

    return 1;
}