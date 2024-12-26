#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "config.h"

typedef struct Player
{
  char wasd;
  float x;
  float y;
  int bullets_fired;
  SDL_Texture *texture;
  SDL_FRect rect;
  int size;
} Player;

void initialize_player(Player *p, SDL_Renderer *renderer);
void handle_input(SDL_Event *e, Player *p);
void update_player_movement(Player *p);

#endif