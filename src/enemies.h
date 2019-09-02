#ifndef ENEMIES_H
#define ENEMIES_H

#include "main.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#define SPAWN_DELAY 3000

void init(void);
void update(void);
bool ifTimePassed(int delay);

#endif