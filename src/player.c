#include "player.h"
#include <SDL2/SDL.h>
#include <math.h>

// player related
typedef struct{

    // movement related
    float x, y;
    bool isTurningLeft, isTurningRight;
    bool isMovingTowards, isMovingBackwards;
    double degrees;
    double acceleration;
    //

    SDL_Texture* sprite;
    SDL_Rect* box;

    const Uint8 *keyboard_state;
    bool isDead;

} Player;

// bullet related
typedef struct{
    float floatX, floatY;
    SDL_Rect bulletSprites[2];
    struct BulletNode *nextBullet; // will be used to create a linked list of bullets
} BulletNode;

BulletNode *bullets;
int shootingDelay;
int startShootTime;
void player_shoot(void);
bool canShoot(void);
//

// converts the player's angles to radians
double deg2rad(double degrees);

static Player player;


void init_player(GameProperties *gameProperties){

    player.sprite = LoadTexture("/home/kibe/Documents/space-shooter/assets/ship.png\0");
    
    // sets player's initial position to the center of the screen
    player.x = (SCREEN_WIDTH - PLAYER_WIDTH) / 2;
    player.y = (SCREEN_HEIGHT - PLAYER_HEIGHT) / 2;

    // allocates enough memory for a SDL_Rect of the player
    player.box = malloc(sizeof(SDL_Rect));
    player.box->w = PLAYER_WIDTH;
    player.box->h = PLAYER_HEIGHT;
    
    // used to detect if a key has been pressed or not
    player.keyboard_state = SDL_GetKeyboardState(NULL);
    
    shootingDelay = 300;

    player.acceleration = 0;
}

// updates and renders anything related to the player
void render_player(SDL_Renderer *renderer){

    // related to player's movement
    if(player.isTurningLeft) player.degrees -= PLAYER_TURNING_SPEED;
    if(player.isTurningRight) player.degrees += PLAYER_TURNING_SPEED;

    player.box->x = player.x;
    player.box->y = player.y;
    
    player.x += sin(deg2rad(player.degrees)) * player.acceleration;
    player.y -= cos(deg2rad(player.degrees)) * player.acceleration;
    
    if(player.isMovingTowards && !player.isMovingBackwards)
    {
        if(player.acceleration < PLAYER_SPEED_LIMIT)
        {
            if(player.acceleration < 0)
                player.acceleration += 0.100;
            else
                player.acceleration += 0.050;
        }
    }
    else if(player.isMovingBackwards && !player.isMovingTowards)
    {
        if(player.acceleration > -PLAYER_SPEED_LIMIT)
        {
            if(player.acceleration > 0)
                player.acceleration -= 0.100;
            else
                player.acceleration -= 0.050;
        }
    }
    else{
        if(player.acceleration > 0)
            player.acceleration -= 0.050;
        else if(player.acceleration < 0)
            player.acceleration += 0.050;
    }

    if(player.degrees == 360 || player.degrees == -360)
        player.degrees = 0;
    //

    // updates and renders bullets
    for(BulletNode *cursor = bullets; cursor != NULL; cursor = cursor->nextBullet){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);     
        for(int i = 0; i < 2; i++){
            SDL_RenderFillRect(renderer, &cursor->bulletSprites[0]);
        }
    }

    // renders player
    SDL_RenderCopyEx(renderer, player.sprite, NULL, player.box, player.degrees, NULL, NULL);

}

// check for inputs (player)
void player_check_inputs(SDL_Event *e){
    
    if(e->type == SDL_KEYDOWN || e->type == SDL_KEYUP){
        
        if(player.keyboard_state[SDL_SCANCODE_SPACE] && canShoot())
            player_shoot(); 
        
        if(player.keyboard_state[SDL_SCANCODE_UP] && !player.keyboard_state[SDL_SCANCODE_DOWN]){
            player.isMovingTowards = true;
        } else { player.isMovingTowards = false; }
        
        if(player.keyboard_state[SDL_SCANCODE_DOWN] && !player.keyboard_state[SDL_SCANCODE_UP]){
            player.isMovingBackwards = true;
        } else { player.isMovingBackwards = false; }
        
        if(player.keyboard_state[SDL_SCANCODE_LEFT] && !player.keyboard_state[SDL_SCANCODE_RIGHT]){
            player.isTurningLeft = true;
        } else { player.isTurningLeft = false; }
        
        if(player.keyboard_state[SDL_SCANCODE_RIGHT] && !player.keyboard_state[SDL_SCANCODE_LEFT]){
            player.isTurningRight = true;
        } else { player.isTurningRight = false; }
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
    free(player.box);
}


bool canShoot(){
    if(SDL_GetTicks() - startShootTime > shootingDelay)
        return true;
    
    return false;
}

double deg2rad(double degrees){
    return (degrees * PI) / 180;
}
