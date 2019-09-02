#include "enemies.h"

int lastTimeEnemySpawn = 0;

typedef struct 
{
	float x, y;
	bool isDead;
	SDL_Rect box;
	struct EnemyNode *nextEnemy; // will be used to create a linked-list of enemies
} EnemyNode;
EnemyNode *enemies;

// forward declarations
bool canSpawn(int delay);
void spawn();


// LEVEL_WIDTH - float.x
// 	x = rand() % (LEVEL_WIDTH+1 - (getGameCamera()->x + SCREEN_WIDTH)) 
								// + getGameCamera()->x + SCREEN_WIDTH;
 
void init(){

}

void update(){

	if(canSpawn(SPAWN_DELAY))
		spawn();

	for(EnemyNode *cursor = enemies; cursor != NULL; cursor = cursor->nextEnemy){
		SDL_SetRenderDrawColor(getGameRenderer(), 255, 255, 255, 255);

		cursor->box.x = (int)(cursor->x - getGameCamera()->x);
		cursor->box.y = (int)(cursor->y - getGameCamera()->y);

		SDL_RenderFillRect(getGameRenderer(), &cursor->box);
	}
}


void spawn(){
	srand(time(0)); // creates a different seed for rand()

	EnemyNode *new_enemy = malloc(sizeof(EnemyNode)); // allocates enough memory for a new enemy
	if(!new_enemy){
		printf("fatal error: [enemy] not allocated");
		setIsGameRunning(false);
	}

	// calculates how much of an off camera space there is on the right and left side of the player
	int rightOffCameraSpace = LEVEL_WIDTH - (getGameCamera()->x + SCREEN_WIDTH);
	int leftOffCameraSpace = SCREEN_WIDTH - rightOffCameraSpace;

	if(rightOffCameraSpace <= 20){ // if there is no space on the right side, spawn enemy on left side
		spawnLeftSide: 
			new_enemy->x = (float)(rand() % ((LEVEL_WIDTH/2)+1 - rightOffCameraSpace+20) + rightOffCameraSpace+20);	
	}
	else if(leftOffCameraSpace <= 20) // if there is no space on the left side, spawn enemy on right side
	{
		spawnRightSide:
			new_enemy->x = (float)(rand() % (LEVEL_WIDTH-20+1 - (SCREEN_WIDTH + leftOffCameraSpace)) + (SCREEN_WIDTH + leftOffCameraSpace));
	}
	else // if there is enough space on both sides
	{
		int whichSide = 0;
		switch(whichSide)
		{
			case 0:
				new_enemy->x = (float)(rand() % (((LEVEL_WIDTH/2)+1) - rightOffCameraSpace+20) + rightOffCameraSpace+20);
				break;
			case 1:
				new_enemy->x = (float)(rand() % (LEVEL_WIDTH-20+1 - (SCREEN_WIDTH + leftOffCameraSpace)) + (SCREEN_WIDTH + leftOffCameraSpace));
				break;
		}
		printf("random side: %i\n", whichSide);
	} 

	new_enemy->y = 0;
	new_enemy->box.w = 20;
	new_enemy->box.h = 20;
	new_enemy->nextEnemy = NULL;

	printf("NEW ENEMY X: %i\n", (int)new_enemy->x);
	printf("CAMERA X: %i\n", getGameCamera()->x);
	printf("LEFT OFF CAMERA SPACE: %i\n", (leftOffCameraSpace));
	printf("RIGHT OFF CAMERA SPACE: %i\n", (rightOffCameraSpace));

	if(!enemies)
		enemies = new_enemy;
	else
	{
		new_enemy->nextEnemy = enemies;
		enemies = new_enemy;
	}

}

// checks if a new enemy can be spawned
bool canSpawn(int delay){

	if(!lastTimeEnemySpawn)
		lastTimeEnemySpawn = SDL_GetTicks();
	else
	{
		if(SDL_GetTicks() - lastTimeEnemySpawn > delay){
			lastTimeEnemySpawn = 0; 
			return true;
		}
	}

	return false;
}