#include "gui.h"

typedef struct{
	SDL_Surface* text;
	SDL_Texture* texture;
	SDL_Rect properties;
}GUIText;

TTF_Font* universalFont;
SDL_Color universalFontColor;

GUIText WavesText;
GUIText TipText;

void init_GUI(){
	// initializes universal variables
	universalFont = LoadFont("/home/kibe/Documents/space-shooter/assets/fonts/Consolas.ttf", 100);
	universalFontColor.r = 255;
	universalFontColor.g = 255;
	universalFontColor.b = 255;
	universalFontColor.a = 255;

	// WAVE TEXT

	WavesText.text = TTF_RenderText_Solid(universalFont, "WAVE", universalFontColor);
	WavesText.texture = SDL_CreateTextureFromSurface(getGameRenderer(), WavesText.text);
	SDL_FreeSurface(WavesText.text);

	WavesText.properties.w = 100;
	WavesText.properties.h = 100;

	// tip text
	TipText.text = TTF_RenderText_Solid(universalFont, "press [SPACE] to shoot", universalFontColor);
	TipText.texture = SDL_CreateTextureFromSurface(getGameRenderer(), TipText.text);
	SDL_FreeSurface(TipText.text);

	TipText.properties.w = 300;
	TipText.properties.h = 15;
	TipText.properties.x = (SCREEN_WIDTH - 300) / 2;
	TipText.properties.y = (SCREEN_HEIGHT / 2) + 200;

}

void render_GUI(){

	if(!playerHasEntered()){
		WavesText.properties.x = getPlayerPos('X', true);
		WavesText.properties.y = (getPlayerPos('Y', true)) + PLAYER_HEIGHT * 2.5;
	}
	else
	{
		if(WavesText.properties.w != 40)
			WavesText.properties.w -= 1;
		if(WavesText.properties.h != 40)
			WavesText.properties.h -= 1;

		if(WavesText.properties.w == 40 && WavesText.properties.h == 40)
		{
			WavesText.properties.x = 10; 
			WavesText.properties.y = 10;
		}

		TipText.properties.w -= 0.1;
		TipText.properties.h -= 0.1;
	}


	SDL_RenderCopy(getGameRenderer(), WavesText.texture, NULL, &WavesText.properties);
	SDL_RenderCopy(getGameRenderer(), TipText.texture, NULL, &TipText.properties);
}