#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

typedef struct {
    bool isRunning;
    SDL_Renderer* renderer;
    SDL_Event* eventHandler;
} GameProperties;



SDL_Texture* LoadTexture(char filePath[]);
#endif
