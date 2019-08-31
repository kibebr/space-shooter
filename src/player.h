#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include <stdbool.h>

#define PLAYER_WIDTH 30
#define PLAYER_HEIGHT 30
#define PLAYER_SPEED 3
#define PLAYER_TURNING_SPEED 3
#define PLAYER_SPEED_LIMIT 5

#define BULLET_WIDTH 5
#define BULLET_HEIGHT 5
#define BULLET_VELOCITY 8
#define SHOOTING_DELAY 300 // milliseconds

void init_player();

void render_player();

void player_check_inputs();

void free_player();

// GETTERS
float getPlayerPos(char POS, bool withOffset);
bool playerHasEntered(void);

#endif
