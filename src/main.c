#include "main.h"
#include "player.h"
#include "gamestates.h"
#include "gui.h"

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

GameProperties gameProperties;
const Uint8 *keyboard_state;

// forward declarations
void render(void);
void check_inputs(void);
void update_camera(void);

typedef struct{
    float x, y;
    SDL_Rect starObject;
} Stars;
static Stars stars[STARS_QUANTITY];

typedef enum {LEFT_WALL=0, TOP_WALL=1, 
                           BOTTOM_WALL=2, RIGHT_WALL=3, 
                                          BORDERS_QUANITY=4} BORDERS;


SDL_Rect borders[BORDERS_QUANITY];

SDL_Rect GameCamera;

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
    

    // initializes the map's walls

    for(int thisWall = 0; thisWall < BORDERS_QUANITY; thisWall++)
    {
        if(thisWall == LEFT_WALL || thisWall == RIGHT_WALL)
        {
            borders[thisWall].y = 0;
            borders[thisWall].h = LEVEL_HEIGHT;
            borders[thisWall].w = 3;
        }
        else // (thisWall == TOP_WALL or thisWall == BOTTOM_WALL)
        {
            borders[thisWall].w = LEVEL_WIDTH;
            borders[thisWall].h = 3;
        }
    }

    // initializes those pretty little stars
    for(int i = 0; i < STARS_QUANTITY; i++){
        stars[i].x = rand()%LEVEL_WIDTH;
        stars[i].y = (rand()%LEVEL_HEIGHT);
        stars[i].starObject.w = STARS_WIDTH;
        stars[i].starObject.h = STARS_HEIGHT;
    }

    // initializes keyboard listener
    keyboard_state = SDL_GetKeyboardState(NULL);

    // initializes player
    init_player();

    // initializes GUI
    init_GUI();

    // initializes the game camera
    GameCamera.x = (LEVEL_WIDTH - SCREEN_WIDTH) / 2;
    GameCamera.y = (LEVEL_HEIGHT - SCREEN_HEIGHT) / 2;
    GameCamera.w = SCREEN_WIDTH;
    GameCamera.h = SCREEN_HEIGHT;

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
            
        if(getCurrentGameState() == RUNNING)
            player_check_inputs(gameProperties.eventHandler);
    }
}

void render(void){

    // renders the background
    SDL_SetRenderDrawColor(gameProperties.renderer, 0, 0, 0, 255);
    SDL_RenderClear(gameProperties.renderer);

    // renders and updates the current game state
    updateCurrentGameState();

    // applies camera offset to the walls and renders them
    borders[LEFT_WALL].x = 0 - GameCamera.x;
    borders[RIGHT_WALL].x = 1276 - GameCamera.x;

    borders[TOP_WALL].y = 0 - GameCamera.y;
    borders[BOTTOM_WALL].y = 957 - GameCamera.y;

    SDL_SetRenderDrawColor(gameProperties.renderer, 255, 0, 0, 100);
    for(int thisWall = 0; thisWall < BORDERS_QUANITY; thisWall++)
        SDL_RenderFillRect(gameProperties.renderer, &borders[thisWall]);

    // renders the stars
    SDL_SetRenderDrawBlendMode(gameProperties.renderer, SDL_BLENDMODE_BLEND);

    for(int i = 0; i < STARS_QUANTITY; i++)
    {
        SDL_SetRenderDrawColor(gameProperties.renderer, 255, 255, 255, rand()%105+150);
        SDL_RenderFillRect(gameProperties.renderer, &stars[i].starObject);

        if(!playerHasEntered())
            stars[i].y += 5;
        else{
            stars[i].y += 0.5;
        }
        
        // if the stars reach the end of the map, set them back up again and randomize their position
        if(stars[i].y > LEVEL_HEIGHT){
            stars[i].x = rand()%LEVEL_WIDTH; 
            stars[i].y = 0;
        }

        // applies camera offset to the stars
        stars[i].starObject.x = stars[i].x - GameCamera.x;
        stars[i].starObject.y = stars[i].y - GameCamera.y;
    }

    // do the following ONLY if the game is currently being played
    if(getCurrentGameState() == RUNNING) 
    {
        render_player(gameProperties.renderer);
        render_GUI();
        if(playerHasEntered())
            update_camera();
    }
    
    // renders... everything
    SDL_RenderPresent(gameProperties.renderer);
}


void update_camera(){

    // centers the game camera on the player
    GameCamera.x = (int)(getPlayerPos('X', false) + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
    GameCamera.y = (int)(getPlayerPos('Y', false) + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

    // if the camera is out of bounds (LEVEL_WIDTH AND LEVEL_HEIGHT NOT SCREEN), stop centering the camera
    if(GameCamera.x < 0)
        GameCamera.x = 0;
    if(GameCamera.y < 0)
        GameCamera.y = 0;
    if(GameCamera.x > (LEVEL_WIDTH - GameCamera.w))
        GameCamera.x = LEVEL_WIDTH - GameCamera.w;
    if(GameCamera.y > (LEVEL_HEIGHT - GameCamera.h))
        GameCamera.y = LEVEL_HEIGHT - GameCamera.h;

}

// GETTERS

SDL_Renderer* getGameRenderer(void){
    return gameProperties.renderer;
}

SDL_Event* getGameEventHandler(void){
    return gameProperties.eventHandler;
}

SDL_Rect* getGameCamera(){
    return &GameCamera;
}

const Uint8* getGameKeyState(void){
    return keyboard_state;
}

// SETTERS

void setIsGamePlaying(bool playing){
    gameProperties.isPlaying = playing;
}

void setIsGameRunning(bool running){
    gameProperties.isRunning = running;
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
       printf("fatal error: could not load image\n");
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

double deg2rad(double degrees){
    return (degrees * PI) / 180;
}
