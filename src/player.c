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
    double velocity;
    //

    SDL_Texture* sprite;
    SDL_Rect box;

    bool isDead;
} Player;

// bullet related
typedef struct{
    float x, y;
    SDL_Rect bulletSprites[2];
    double currentPlayerDegrees;   
    double currentPlayerVelocity;
    struct BulletNode *nextBullet; // will be used to create a linked list of bullets
} BulletNode;

BulletNode *bullets;

int startedShooting;
int lastTimeShot;

void player_shoot(void);
void player_update_bullets(void);
bool canShoot(void);
//

bool alreadyEntered = false;
bool playerHasEntered(void);

// collision related
typedef enum { BOUNDARIES, ENEMY, POWER_UP } COLLISION_TYPE;
typedef enum { TOP=0, BOTTOM=1, LEFT=2, RIGHT=3 } COLLISIONBOX_POS;
bool collidesWith(COLLISION_TYPE cType);


static Player player;

// initializes player
void init_player(){
    
    // loads player's sprite
    player.sprite = LoadTexture("/home/kibe/Documents/space-shooter/assets/ship.png\0");
    
    // sets player's initial position to the center of the screen
    player.x = (LEVEL_WIDTH - PLAYER_WIDTH) / 2;
    player.y = ((LEVEL_HEIGHT - PLAYER_HEIGHT) / 2) + 230;

    // sets player's width and height
    player.box.w = PLAYER_WIDTH;
    player.box.h = PLAYER_HEIGHT;

    alreadyEntered = false;
}

// updates and renders anything related to the player
void render_player(){
    if(!playerHasEntered())
        player.velocity = 0.5;

    // turns the player
    printf("%i\n", alreadyEntered);
    if(player.isTurningLeft && playerHasEntered()) player.degrees -= PLAYER_TURNING_SPEED;
    if(player.isTurningRight && playerHasEntered()) player.degrees += PLAYER_TURNING_SPEED;
    
    // sets sprite's X and Y to player's original X and Y and applies camera offset
    player.box.x = (int)(player.x - getGameCamera()->x);
    player.box.y = (int)(player.y - getGameCamera()->y);

    // movement
    player.x += sin(deg2rad(player.degrees)) * player.velocity;
    player.y -= cos(deg2rad(player.degrees)) * player.velocity;

    if(player.isMovingTowards && !player.isMovingBackwards)
    {
        if(player.velocity < PLAYER_SPEED_LIMIT)
        {
            if(player.velocity < 0)
                player.velocity += 0.100;
            else
                player.velocity += 0.050;
        }
    }
    else if(player.isMovingBackwards && !player.isMovingTowards)
    {
        if(player.velocity > (-PLAYER_SPEED_LIMIT + 3))
        {
            if(player.velocity > 0)
                player.velocity -= 0.100;
            else
                player.velocity -= 0.050;
        }
        else
            player.velocity += 0.050;
    }
    else{
        if(player.velocity > 0)
            player.velocity -= 0.050;
        else if(player.velocity < 0)
            player.velocity += 0.050;
    }

    if(player.degrees == 360 || player.degrees == -360)
        player.degrees = 0;
    //

    if(collidesWith(BOUNDARIES))
        player.velocity *= -1;
    

    SDL_SetRenderDrawColor(getGameRenderer(), 255, 255, 255, 255);
    // renders player
    SDL_RenderCopyEx(getGameRenderer(), player.sprite, NULL, &player.box, player.degrees, NULL, NULL);

    player_update_bullets();
}

// check for inputs (player)
void player_check_inputs(){
    
    if(getGameEventHandler()->type == SDL_KEYDOWN || getGameEventHandler()->type == SDL_KEYUP){
        
        if(getGameKeyState()[SDL_SCANCODE_SPACE] && canShoot())
            player_shoot(); 
        
        if(getGameKeyState()[SDL_SCANCODE_UP] && !getGameKeyState()[SDL_SCANCODE_DOWN]){
            player.isMovingTowards = true;
        } else { player.isMovingTowards = false; }
        
        if(getGameKeyState()[SDL_SCANCODE_DOWN] && !getGameKeyState()[SDL_SCANCODE_UP]){
            player.isMovingBackwards = true;
        } else { player.isMovingBackwards = false; }
        
        if(getGameKeyState()[SDL_SCANCODE_LEFT] && !getGameKeyState()[SDL_SCANCODE_RIGHT]){
            player.isTurningLeft = true;
        } else { player.isTurningLeft = false; }
        
        if(getGameKeyState()[SDL_SCANCODE_RIGHT] && !getGameKeyState()[SDL_SCANCODE_LEFT]){
            player.isTurningRight = true;
        } else { player.isTurningRight = false; }
    }
}

void player_shoot(){

    // creates a node that will contain the new bullet
    BulletNode *new_bullet = malloc(sizeof(BulletNode));
    if(!new_bullet)
    { 
        printf("fatal error: [pbullets] memory not allocated\n");
        setIsGameRunning(false);
    }
    
    // the new bullet properties
    lastTimeShot = SDL_GetTicks();
    new_bullet->currentPlayerDegrees = player.degrees;
    new_bullet->currentPlayerVelocity = player.velocity;
    new_bullet->nextBullet = NULL;

    // creates the sprites (basically rectangles) that will be the bullets
    for(int i = 0; i < 2; i++){
        new_bullet->bulletSprites[i].w = BULLET_WIDTH;
        new_bullet->bulletSprites[i].h = BULLET_HEIGHT;
        new_bullet->x = player.x + 10;
        new_bullet->y = player.y + 10;
    }
    
    if(!bullets) { bullets = new_bullet; }
    else
    {
        new_bullet->nextBullet = bullets;
        bullets = new_bullet;
    }
    
    player.velocity -= 0.90;
}

void player_update_bullets(void){

    /*
        UPDATES AND RENDERS BULLETS
        basically, cursor is a pointer that will knock on the door of every bullet node, 
        accessing the nextBullet pointer to find the next door (bullet), to ask for  
        their respective values. the loop goes on until there are no bullets left.
    */

    for(BulletNode *cursor = bullets; cursor != NULL; cursor = cursor->nextBullet){
        SDL_SetRenderDrawColor(getGameRenderer(), 255, 255, 255, 255);     
        for(int i = 0, n_of_bullets = 2; i < n_of_bullets; i++){
            SDL_RenderFillRect(getGameRenderer(), &cursor->bulletSprites[0]);
            cursor->x += sin(deg2rad(cursor->currentPlayerDegrees)) * (BULLET_VELOCITY);
            cursor->y -= cos(deg2rad(cursor->currentPlayerDegrees)) * (BULLET_VELOCITY);

            // applies the camera offset to the bullets
            cursor->bulletSprites[i].x = cursor->x - getGameCamera()->x;
            cursor->bulletSprites[i].y = cursor->y - getGameCamera()->y;
        }
    }
}

void free_player(){

}

bool playerHasEntered(){
    if((int)getPlayerPos('Y', false) == ((LEVEL_HEIGHT - PLAYER_HEIGHT) / 2))
    {
        alreadyEntered = true;
    }


    return alreadyEntered;
}

bool collidesWith(COLLISION_TYPE cType){
    switch(cType)
    {
        case BOUNDARIES:
            if(player.x < 0)
                return true;
            if(player.y < 0)
                return true;
            if(player.y > (LEVEL_HEIGHT - PLAYER_HEIGHT))
                return true;
            if(player.x > (LEVEL_WIDTH - PLAYER_WIDTH))
                return true;
            break;
    }

    return false;
}

bool canShoot(){
    if(SDL_GetTicks() - lastTimeShot > SHOOTING_DELAY)
        return true;
    
    return false;
}

float getPlayerPos(char POS, bool withOffset){
    if(POS == 'X')
        if(withOffset)
            return (float)player.box.x;
        else
            return player.x;
    else
        if(withOffset)
            return (float)player.box.y;
        else
            return player.y;
}