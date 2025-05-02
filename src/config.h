#ifndef CONFIG_H
#define CONFIG_H

/**
 * MASTER GAME CONFIGURATIONS
 * This will eventually turn into a settings page or something of that sort.
 */

/**
 * ENVIRONMENT
 */
#define SCREEN_WIDTH 1500.0f
#define SCREEN_HEIGHT 900.0f
#define STAR_COUNT 100
#define STAR_MAX_SIZE 5.0f
#define STAR_MAX_VELOCITY 10.0f

/**
 * GAME
 */
#define SHIP_SIZE 30
#define BULLET_SPEED 15.0f

/**
 * PLAYER
 */
#define PLAYER_SPEED 6.0f
#define PLAYER_NUM_BULLETS 50
#define PLAYER_DEFAULT_LIVES 3
#define PARTICLE_TRAIL_COUNT 20
// #define PLAYER_WRAP_AROUND

/**
 * ENEMY
 */
#define ENEMIES_COUNT 50
#define ENEMY_MAX_CLUSTER_SIZE 50
#define ENEMY_SPEED 2.0f
#define ENEMY_MAX_HEALTH 100
#define ENEMY_BULLET_BUFFER_SIZE 0
#define DEFAULT_ENEMY_BMP "assets/enemy.bmp"

#endif