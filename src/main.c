#include "main.h"
#include "player.h"
#include "gamestates.h"

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

GameProperties gameProperties;

// forward declarations
void render(void);
void check_inputs(void);

SDL_Rect stars[STARS];

int main(void){
    CURRENT_GAME_STATE = MAIN_MENU;

    gameProperties.isRunning = true; // game is now running
    gameProperties.isPlaying = false; // player is at the main menu

    // inits SDL
    SDL_Init(SDL_INIT_VIDEO);
    
    // inits TTF
    TTF_Init();

    // declares the window
    SDL_Window* window = SDL_CreateWindow("WIP",    SDL_WINDOWPOS_UNDEFINED,
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
    init_player();

    // initializes the game states (main menu, etc.)
    init_game_states();

    // basic game loop
    while(gameProperties.isRunning){
        check_inputs();
        render();
        SDL_Delay(16);
    }
    
    // those functions are called when the game is closed
    free(gameProperties.eventHandler);   
    free_player();
    SDL_Quit();
    TTF_Quit();
    return 1;
}



void check_inputs(void)
{
    // poll events and checks if user quits the game
    while(SDL_PollEvent(gameProperties.eventHandler))
    {

        if(gameProperties.eventHandler->type == SDL_QUIT)
            gameProperties.isRunning = false;
            
        if(gameProperties.isPlaying)
            player_check_inputs(gameProperties.eventHandler);
    }
}

void render(void){

    // renders the background
    SDL_SetRenderDrawColor(gameProperties.renderer, 0, 0, 0, 255);
    SDL_RenderClear(gameProperties.renderer);

    // renders and updates the current game state
    updateCurrentGameState();

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
    
    // renders the player if the game is in RUNNING state
    if(gameProperties.isPlaying)
        render_player(gameProperties.renderer);
    
    // renders... everything
    SDL_RenderPresent(gameProperties.renderer);
}


// GETTERS

SDL_Renderer* getGameRenderer(void){
    return gameProperties.renderer;
}


/*
    GAME UTILITIES

    LoadTexture() -> used to automatically create a surface from an image,
    transform it into texture and returns it
    --
    LoadFont() -> used to automatically create a surface from a font,
    transform it into texture and returns it

*/

SDL_Texture* LoadTexture(char filePath[]){
   SDL_Surface* surface = IMG_Load(filePath);
   if(!surface){
       printf("fatal error: not possible to load image\n");
       printf("image path: %s\n", filePath);
       gameProperties.isRunning = false;
   }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gameProperties.renderer,                                                                                    surface);
    SDL_FreeSurface(surface);
    return texture;
} 

TTF_Font* LoadFont(char filePath[], int fontSize)
{
    TTF_Font *tmp = TTF_OpenFont(filePath, fontSize);
    if(!tmp)
    {
        printf("fatal error: could not load font\n");
        printf("font path: %s\n", filePath);
        gameProperties.isRunning = false;
    }
    
    return tmp;
}