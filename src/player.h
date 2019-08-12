#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include <stdbool.h>

#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 40
#define PLAYER_SPEED 3
#define PLAYER_TURNING_SPEED 3
#define PLAYER_SPEED_LIMIT 5

#define BULLET_WIDTH 5
#define BULLET_HEIGHT 5
#define BULLET_VELOCITY 8
#define SHOOTING_DELAY 300 // milliseconds

#define PI 3.141592653589793238462643383279502884

void init_player();

void render_player(SDL_Renderer *renderer);

void player_check_inputs(SDL_Event *e);

void free_player();

#endif
