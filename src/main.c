#include "main.h"
#include "player.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define STARS 200
#define STARS_WIDTH 2
#define STARS_HEIGHT 2

GameProperties gameProperties;
void render(void);

SDL_Rect stars[STARS];

int main(void){
    gameProperties.isRunning = true; // game is now running
    
    // inits SDL
    SDL_Init(SDL_INIT_VIDEO);
    
    // declares the window
    SDL_Window* window = SDL_CreateWindow("WIP", SDL_WINDOWPOS_UNDEFINED,
                                                    SDL_WINDOWPOS_UNDEFINED,
                                                    SCREEN_WIDTH,
                                                    SCREEN_HEIGHT, 0);
    // declares the renderer
    gameProperties.renderer = SDL_CreateRenderer(window, -1,                                                                        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE); 
     
    // allocates enough space for an event handler (game-wide)
    gameProperties.eventHandler = malloc(sizeof(SDL_Event));
    
    // initializes those pretty little stars
    for(int i = 0; i < STARS; i++){
        stars[i].x = rand()%SCREEN_WIDTH;
        stars[i].y = rand()%SCREEN_HEIGHT;
        stars[i].w = STARS_WIDTH;
        stars[i].h = STARS_HEIGHT;
    }
    
    // initializes player
    init_player(&gameProperties);
    
    // basic game loop
    while(gameProperties.isRunning){
        
        // poll events and checks if user quits the game
        while(SDL_PollEvent(gameProperties.eventHandler)){
            if(gameProperties.eventHandler->type == SDL_QUIT)
                gameProperties.isRunning = false;
            
            player_check_inputs(gameProperties.eventHandler);
        }
        render();
        SDL_Delay(16);
    }
    
    free(gameProperties.eventHandler);   
    free_player();
    SDL_Quit();
    return 1;
}



void render(void){
    
    // renders the background
    SDL_SetRenderDrawColor(gameProperties.renderer, 0, 0, 0, 255);
    SDL_RenderClear(gameProperties.renderer);

    
    // renders the stars
    SDL_SetRenderDrawBlendMode(gameProperties.renderer, SDL_BLENDMODE_BLEND);
    for(int i = 0; i < STARS; i++){
        SDL_SetRenderDrawColor(gameProperties.renderer, 255, 255, 255, rand()%105+150);
        SDL_RenderFillRect(gameProperties.renderer, &stars[i]);
        stars[i].y++;
        
        if(stars[i].y > SCREEN_HEIGHT){
            stars[i].x = rand()%SCREEN_WIDTH; // randomize their position again
            stars[i].y = 0;
        }

    }
    
    
    // renders the player
    render_player(gameProperties.renderer);
    
    // renders... everything
    SDL_RenderPresent(gameProperties.renderer);
}

// public
SDL_Texture* LoadTexture(char filePath[]){
   SDL_Surface* surface = IMG_Load(filePath);
   if(!surface){
       printf("fatal error: not possible to load image\n");
       printf("file name: %s\n", filePath);
       gameProperties.isRunning = false;
   }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gameProperties.renderer,                                                                                    surface);
    SDL_FreeSurface(surface);
    return texture;
}
