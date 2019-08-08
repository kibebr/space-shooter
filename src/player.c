#include "player.h"
#include <SDL2/SDL.h>
#include <time.h>

static SDL_Texture* playerSprite;
static SDL_Rect* playerBox;
static Player player;
static bool up, down, left, right;
static const Uint8 *keyboard_state;

// struct for the bullet
typedef struct{
    SDL_Rect bulletSprites[2];
    struct BulletNode *nextBullet; // will be used to create linked list of bullets
} BulletNode;

// bullet related
BulletNode *bullets;
int shootingDelay;
int startShootTime;
void player_shoot(void);
bool canShoot(void);
//

void init_player(GameProperties *gameProperties){
    player.isDead = false;
    playerSprite = LoadTexture("/home/kibe/Desktop/space_test/assets/ship.png\0");
    
    // sets player's initial position to the center of the screen
    player.x = (SCREEN_WIDTH - PLAYER_WIDTH) / 2;
    player.y = (SCREEN_HEIGHT - PLAYER_HEIGHT) / 2;
    
    // allocates enough memory for a SDL_Rect of the player
    playerBox = malloc(sizeof(SDL_Rect));
    playerBox->w = PLAYER_WIDTH;
    playerBox->h = PLAYER_HEIGHT;
    
    // used to detect if a key has been pressed or not
    keyboard_state = SDL_GetKeyboardState(NULL);
    
    shootingDelay = 300;
}

// updates and renders anything related to the player
void render_player(SDL_Renderer *renderer){
    if(up && player.y > 0) { player.y -= PLAYER_SPEED; }
    if(down && player.y < SCREEN_HEIGHT - PLAYER_HEIGHT) { player.y += PLAYER_SPEED; }
    if(left && player.x > 0) { player.x -= PLAYER_SPEED; }
    if(right && player.x < SCREEN_WIDTH - PLAYER_WIDTH) { player.x += PLAYER_SPEED; } 
    
    playerBox->x = player.x;
    playerBox->y = player.y;
    
    SDL_RenderCopy(renderer, playerSprite, NULL, playerBox);
    
    // updates and renders bullets
    for(BulletNode *cursor = bullets; cursor != NULL; cursor = cursor->nextBullet){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);     
        for(int i = 0; i < 2; i++){
            SDL_RenderFillRect(renderer, &cursor->bulletSprites[i]);
            cursor->bulletSprites[i].y -= 5;
        }
    }
}

// check for inputs (player)
void player_check_inputs(SDL_Event *e){
    
    if(e->type == SDL_KEYDOWN || e->type == SDL_KEYUP){
        
        if(keyboard_state[SDL_SCANCODE_SPACE] && canShoot())
            player_shoot(); 
        
        if(keyboard_state[SDL_SCANCODE_UP] && !keyboard_state[SDL_SCANCODE_DOWN]){
            up = true;
        } else { up = false; }
        
        if(keyboard_state[SDL_SCANCODE_DOWN] && !keyboard_state[SDL_SCANCODE_UP]){
            down = true;
        } else { down = false; }
        
        if(keyboard_state[SDL_SCANCODE_LEFT] && !keyboard_state[SDL_SCANCODE_RIGHT]){
            left = true;
        } else { left = false; }
        
        if(keyboard_state[SDL_SCANCODE_RIGHT] && !keyboard_state[SDL_SCANCODE_LEFT]){
            right = true;
        } else { right = false; }
    }
}

void player_shoot(){
    startShootTime = SDL_GetTicks();
    // creates a node that will contain the new bullet
    BulletNode *new_bullet = malloc(sizeof(BulletNode));
    if(!new_bullet) { printf("fatal error: [pbullets] memory not allocated\n");}

    // creates the sprites (basically rectangles) that will be the bullets
    for(int i = 0; i < 2; i++){
        new_bullet->bulletSprites[i].w = BULLET_WIDTH;
        new_bullet->bulletSprites[i].h = BULLET_HEIGHT;
    }
    
    new_bullet->bulletSprites[0].x = player.x - 2;
    new_bullet->bulletSprites[0].y = player.y;
    new_bullet->bulletSprites[1].x = player.x + 25;
    new_bullet->bulletSprites[1].y = player.y;    
    
    // once the player shoots again, we will replace the nextBullet with a new one
    new_bullet->nextBullet = NULL;
    
    if(!bullets) { bullets = new_bullet; }
    else
    {
        new_bullet->nextBullet = bullets;
        bullets = new_bullet;
    } 
}

void free_player(){
    free(playerBox);
}

bool canShoot(){
    if(SDL_GetTicks() - startShootTime > shootingDelay)
        return true;
    
    return false;
}
