#include "myLib.h"

static char paused[7] = "PAUSED";
int main(){
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	drawImage(0, 0, 240, 150, menu_data);//draws menu with DMA for credit.
	//loop for entire game. Allows repeatedly displaying the menu.
	while(1){
		drawMenu();
		setStringColor(RGB(20, 5, 5));

		//draws start instruction
		sprintf(scoreMsg, "PRESS START");
		drawRect(56, 87, 66, 8, BLACK);
		drawString(56, 87, scoreMsg, YELLOW);

		//seeds rand()
		int seed;
		while(!KEY_DOWN_NOW(BUTTON_A) && !KEY_DOWN_NOW(BUTTON_START)){
			seed++;
		}
		srand(seed);

		resetVars();
		drawBackground();

		//waits until unpress start, otherwise pauses immediately
		while(KEY_DOWN_NOW(BUTTON_START));

		while(gameInProgress){//main game loop			

			//spawns zombies on timer. More often as level increases.
			//Also needs to make sure map isn't full,
			if(timer == spawnRate){ //time to spawn
				timer = 0;
				if(numZoms < max){//map not full
					spawnZombie();
				}
			} else{
				timer++;
			}

			//pauses the game if press start.
			if(KEY_DOWN_NOW(BUTTON_START)){//first press
				drawStringFast(72, 102, paused);
				while(KEY_DOWN_NOW(BUTTON_START)){//while first pressed
					waitForVblank();
					waitForVblank();
				}
				while(!KEY_DOWN_NOW((BUTTON_START))){// while lifted up
					waitForVblank();
					waitForVblank();
				}
				//unpauses after press start again
				while(KEY_DOWN_NOW(BUTTON_START)){//while second pressed
					waitForVblank();
					waitForVblank();
				}
				drawRectIMG(72, 102, 36, 8, background_data);
			}

			//figure out next frame
			updateDirections();
			updatePlayer();
			updateBullets();
			//sprintf(debugNumZoms, "LC = %d", levelCounter);
			sprintf(scoreMsg, "%d", score);//update score string
			sprintf(levelMsg, "Level %d", level);
			moveZombies();

			waitForVblank();

			//draw frame
			drawStringFast(1, 5, scoreMsg);
			drawStringFast(1, 191, levelMsg);
			drawBullets();
			drawPlayer();
			drawZombies();

			//Returns to main menu if press select.
			if(KEY_DOWN_NOW(BUTTON_SELECT)){
				gameInProgress = 0;
				drawRect(150, 0, 240, 10, BLACK);
				sprintf(scoreMsg, "Score: %d", score);//update message
				drawString(151, 5, scoreMsg, RGB(20, 5, 5));
			}

			//Plays Game Over "Animation" if died.
			if(lost){
				gameOver();
			}
		}

		
		//Goes from Game over screen to main menu if press Select/Start
		while(lost){
			if(KEY_DOWN_NOW(BUTTON_START) || KEY_DOWN_NOW(BUTTON_SELECT)){
				lost = 0;
			}

		}
	}//end main loop
}//end main method



/*void delay(int n)
{
	volatile int size = 0;
	for(int i=0; i<n*4000; i++)
	{
		size = size + 1;
	}
}*/

/**
*Waits until start of next Vertical blank
*/
void waitForVblank()
{
	while(SCANLINECOUNTER > 161);
	while(SCANLINECOUNTER < 160);
} 

/**
*Reinitializes all important game variables so consecutive games are independent.
*/
void resetVars(){
	gameInProgress = 1; //start game
	lost = 0;
	score = 0;
	timer = 0;
	level = 1;
	numZoms = 0;
	levelCounter = 5;
	row = 80;
	col = 80;
	spawnRate = 75 - level*5;//set spawnRate
	for(int i = 0; i < max; i++){//reset all zombies & bulets
				zombies[i].col = 0;
				zombies[i].row = 0;
				zombies[i].oldRow = row;
				zombies[i].oldCol = col;
				zombies[i].health = 0;
				bullets[i].health = 0;
			}	

}
