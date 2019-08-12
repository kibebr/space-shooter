#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define STARS 200
#define STARS_WIDTH 2
#define STARS_HEIGHT 2

typedef struct {
    bool isRunning;
    bool isPlaying;
    SDL_Renderer* renderer;
    SDL_Event* eventHandler;
} GameProperties;


// GETTERS
SDL_Renderer* getGameRenderer(void);

// GAME UTILITIES
SDL_Texture* LoadTexture(char filePath[]);
TTF_Font* LoadFont(char filePath[], int fontSize);

#endif
