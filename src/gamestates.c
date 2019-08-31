#include "gamestates.h"
#include "menu.h"
#include "main.h"
#include <SDL2/SDL.h>

static enum GAME_STATE CURRENT_GAME_STATE = MAIN_MENU;

void init_game_states(){
	init_main_menu();
}

void updateCurrentGameState(){
	switch(CURRENT_GAME_STATE)
	{
		case MAIN_MENU:
			update_main_menu();
			break;
		case RUNNING:
			setIsGamePlaying(true);
			break;
	}
}

enum GAME_STATE getCurrentGameState()
{
	return CURRENT_GAME_STATE;
}

void setCurrentGameState(int state){
	CURRENT_GAME_STATE = (enum GAME_STATE) state;
}