#ifndef GAME_STATES_H
#define GAME_STATES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

enum GAME_STATE { MAIN_MENU, RUNNING, PAUSED,
                   			GAME_OVER         };

static enum GAME_STATE CURRENT_GAME_STATE;

void init_game_states();

void updateCurrentGameState();

enum GAME_STATE getCurrentGameState(void);

#endif