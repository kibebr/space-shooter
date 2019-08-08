#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include <stdbool.h>

#define PLAYER_WIDTH 30
#define PLAYER_HEIGHT 30
#define PLAYER_SPEED 3

#define BULLET_WIDTH 5
#define BULLET_HEIGHT 5

typedef struct{
    bool isDead;
    double x, y;
} Player;

void init_player(GameProperties *gameProperties);

void render_player(SDL_Renderer *renderer);

void player_check_inputs(SDL_Event *e);

void free_player();

#endif
