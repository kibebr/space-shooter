#include "main.h"
#include "gamestates.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

// every object in the menu will have those variables
typedef struct
{
	SDL_Surface* text;
	SDL_Texture* texture;
	SDL_Rect properties;
} MenuObject;

// default variables
TTF_Font* menuFont;
SDL_Color menuFontColor;

MenuObject MenuTitle;

/*

	*MENU OPTIONS*
	0 = START
	1 = OPTIONS
	2 = EXIT

*/
int currentOption;
int selectDelay, lastTimeSelected;
MenuObject MenuOptions[3];
MenuObject MenuArrows[2];

// forward declarations
void menu_check_inputs();
bool canSelect(int lastTimeSelected);


void init_main_menu(){

	// initialize default variables
	menuFont = LoadFont("/home/kibe/Documents/space-shooter/assets/fonts/Consolas.ttf", 28);
	menuFontColor.r = 255;
	menuFontColor.g = 255;
	menuFontColor.b = 255;
	menuFontColor.a = 255;

	// title properties
	MenuTitle.text = TTF_RenderText_Solid(menuFont, "ASTEROIDS", 
													 menuFontColor);
	MenuTitle.texture = SDL_CreateTextureFromSurface(getGameRenderer(), 
													 MenuTitle.text);
	SDL_FreeSurface(MenuTitle.text);

	MenuTitle.properties.w = 290;
	MenuTitle.properties.h = 100;
	MenuTitle.properties.x = (SCREEN_WIDTH - MenuTitle.properties.w) / 2; 
	MenuTitle.properties.y = (SCREEN_HEIGHT - MenuTitle.properties.h) / 2 - 100;
	//


	// options properties
	const char *options[] = { "START", "SETTINGS", "EXIT" };
	currentOption = 0;
	selectDelay = 100;
	lastTimeSelected = 0;

	for(int i=0, nOfOptions=3; i<nOfOptions; i++)
	{ 

		MenuOptions[i].text = TTF_RenderText_Solid(menuFont, options[i], menuFontColor);
		MenuOptions[i].texture = SDL_CreateTextureFromSurface(getGameRenderer(), MenuOptions[i].text);
		SDL_FreeSurface(MenuOptions[i].text);
		MenuOptions[i].properties.w = 80;
		MenuOptions[i].properties.h = 30;
		MenuOptions[i].properties.x = (SCREEN_WIDTH - MenuOptions[i].properties.w) / 2;
		MenuOptions[i].properties.y = (SCREEN_HEIGHT - MenuOptions[i].properties.h) / 2 + 100 * i;
	}
	

	// arrows properties
	MenuArrows[0].text = TTF_RenderText_Solid(menuFont, ">", menuFontColor);
	MenuArrows[1].text = TTF_RenderText_Solid(menuFont, "<", menuFontColor);

	for(int i=0, nOfArrows=2; i<nOfArrows; i++){
		MenuArrows[i].texture = SDL_CreateTextureFromSurface(getGameRenderer(), MenuArrows[i].text);
		SDL_FreeSurface(MenuArrows[i].text);

		MenuArrows[i].properties.w = 15;
		MenuArrows[i].properties.h = 15;
	}
}

void update_main_menu(){

	// renders the title
	SDL_RenderCopy(getGameRenderer(), MenuTitle.texture, NULL, &MenuTitle.properties);

	// renders the options
	for(int i=0, nOfOptions=3; i < nOfOptions; i++)
		SDL_RenderCopy(getGameRenderer(), MenuOptions[i].texture, NULL, &MenuOptions[i].properties);
	
	// renders the arrows
	MenuArrows[0].properties.x = MenuOptions[currentOption].properties.x - 20;
	MenuArrows[1].properties.x = MenuOptions[currentOption].properties.x + MenuOptions[currentOption].properties.w;
	for(int i=0, nOfArrows=2; i < nOfArrows; i++){
		SDL_RenderCopy(getGameRenderer(), MenuArrows[i].texture, NULL, &MenuArrows[i].properties);
		MenuArrows[i].properties.y = MenuOptions[currentOption].properties.y + 5;
	}

	menu_check_inputs();
}

void menu_check_inputs(){
	
	if(getGameEventHandler()->type == SDL_KEYDOWN){
		if(getGameKeyState()[SDL_SCANCODE_DOWN] && currentOption != 2 && canSelect(lastTimeSelected)){
			currentOption++;
			lastTimeSelected = SDL_GetTicks();
		}
		else if(getGameKeyState()[SDL_SCANCODE_UP] && currentOption != 0 && canSelect(lastTimeSelected))
		{
			currentOption--;
			lastTimeSelected = SDL_GetTicks();
		}
		else if(getGameKeyState()[SDL_SCANCODE_RETURN])
		{
			switch(currentOption)
			{
				case 0:
					setCurrentGameState(1);
					break;
				case 2:
					setIsGameRunning(false);
					break;
			}
		}
	}
}

bool canSelect(int lastTimeSelected){
	if(SDL_GetTicks() - lastTimeSelected > selectDelay)
		return true;

	return false;
}