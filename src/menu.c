#include "main.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef struct
{
	TTF_Font* font;
	SDL_Color color;
	SDL_Surface* text;
	SDL_Texture* texture;
	SDL_Rect properties;
} MenuObjects;

MenuObjects MenuTitle;


void init_main_menu(){

	// initializes the title and sets its positioning

	MenuTitle.font = LoadFont("/home/kibe/Documents/space-shooter/assets/fonts/Consolas.ttf", 24);

	MenuTitle.color.r = 255;
	MenuTitle.color.g = 255;
	MenuTitle.color.b = 255;
	MenuTitle.color.a = 255;

	MenuTitle.text = TTF_RenderText_Solid(MenuTitle.font, "ASTEROIDS", 
													 MenuTitle.color);
	MenuTitle.texture = SDL_CreateTextureFromSurface(getGameRenderer(), 
													 MenuTitle.text);
	SDL_FreeSurface(MenuTitle.text);
	MenuTitle.properties.w = 200;
	MenuTitle.properties.h = 50;
	MenuTitle.properties.x = (SCREEN_WIDTH - MenuTitle.properties.w) / 2; 
	MenuTitle.properties.y = (SCREEN_HEIGHT - MenuTitle.properties.h) / 2;
}

void update_main_menu(){
	SDL_RenderCopy(getGameRenderer(), MenuTitle.texture, NULL, &MenuTitle.properties);
	SDL_RenderPresent(getGameRenderer());
}