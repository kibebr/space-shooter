#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define LEVEL_WIDTH 1280
#define LEVEL_HEIGHT 960

#define STARS_QUANTITY 500
#define STARS_WIDTH 2
#define STARS_HEIGHT 2

#define PI 3.141592653589793238462643383279502884

typedef struct {
    bool isRunning;
    bool isPlaying;
    SDL_Renderer* renderer;
    SDL_Event* eventHandler;
} GameProperties;

// GETTERS
SDL_Renderer* getGameRenderer(void);
SDL_Event* getGameEventHandler(void);
SDL_Rect* getGameCamera(void);
const Uint8* getGameKeyState(void);

// SETTERS
void setIsGamePlaying(bool playing);
void setIsGameRunning(bool running);

// GAME UTILITIES
SDL_Texture* LoadTexture(char filePath[]);
TTF_Font* LoadFont(char filePath[], int fontSize);
double deg2rad(double degrees); // converts degrees to radians

#endif
