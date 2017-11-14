#include "myLib.h"

//atic vars
static int deadZoms[15];//boolean list of if bullet died this frame



/**
*Updates all of the zombies' positions and directions. Doesn't draw zombies
**/
void updateZombies(){
	updateDirections();
	moveZombies();
	//TODO??
} 
/**
*Sets all of the zombies pointing towards the player
**/
void updateDirections(){
	for(int i = 0; i < max; i++){
		if(zombies[i].health){
			OBJECT *cur = (zombies+i);
			cur->xDir = 0;
			cur->yDir = 0;
			if(row > cur->row){//player above zombie
				cur->yDir = zSpeed;
			} else if(row < cur->row){//player below zombie
				cur->yDir = -zSpeed;
			}
			if(col > cur->col){//player right of zombie
				cur->xDir = zSpeed;
			} else if(col < cur->col){//player left of Zombie
				cur->xDir = -zSpeed;
			}
			//zombies[i].xDir = cur->xDir;//save xDir maybe dont need??
			//zombies[i].yDir = cur->yDir;//save yDir
		}
	}//end loop
}

/**
*Moves all the zombies. Deals with collision.
**/
void moveZombies(){
	//TODO
	for(int i = 0; i < max; i++){
		if(zombies[i].health){
			//only move zombie every other frame. Slows them down by half.
			if(zombies[i].row == zombies[i].oldRow && zombies[i].col == zombies[i].oldCol){
				//if didn't move last frame
				correctCollision(i);//makes sure zombies collide
				zombies[i].row += zombies[i].yDir;
				zombies[i].col += zombies[i].xDir;
			} 
			else{
				//if did move last frame
				zombies[i].oldRow = zombies[i].row; //oldRow = row
				zombies[i].oldCol = zombies[i].col; ///oldCol = col
			}
			
			
		}
	}

}

/**
*Corrects the x and y direction of the zombie at the given index,
*so it collides with non-black pixels
**/
void correctCollision(int index){
	
	if(index >= 0){//is a zombie
		int zRow = zombies[index].row;//loads relevent zombie vars
		int zCol = zombies[index].col;
		int zdirX = zombies[index].xDir;//get xDir
		int zdirY = zombies[index].yDir;
		borderCheck(index);
		//check for collision
		//need up and down to work.
		if(zRow < 239 && zRow > 9){
			if(zombies[index].isBoss){
				zombieCollision(index);//Player can't fit inside boss zombie
				zombies[index].yDir = verticalCollision(zRow, zCol, zdirY, bossZSize);//sets y dir to 0
				zombies[index].xDir = horizontalCollision(zRow, zCol, zdirX, bossZSize);
			}
			else{
				zombies[index].yDir = verticalCollision(zRow, zCol, zdirY, zSize);//sets y dir to 0
				zombies[index].xDir = horizontalCollision(zRow, zCol, zdirX, zSize);
			}
		}
	}
}

/*
*Returns the corrected x direction of the object with the given
*row, column, x component direction, and size. Assumes is square.
*/
int horizontalCollision(int row, int col, int xDir, int size){
	int isCollision = 0;//value of the corner pixels being checked
	int pixel;//location (in offset) of pixel being checked

	if(xDir < 0){//checks left if moves left
			for(int i = -1; i >= xDir; i--){//checks every pixel between start & end
				pixel = OFFSET(row, col + i, 240);//left top
				isCollision = isCollision | (background_data[pixel] ^ videoBuffer[pixel]);					
				pixel = OFFSET(row + size - 1, col + i, 240);//left bottom
				isCollision = isCollision | (background_data[pixel] ^ videoBuffer[pixel]);

			}					
		} else if(xDir > 0){//checks right if moves right
			for(int i = 1; i <= xDir; i++){//checks every pixel between start & end
				pixel = OFFSET(row, col + size -1 + i, 240);//right top
				isCollision = isCollision | (background_data[pixel] ^ videoBuffer[pixel]);	
				pixel = OFFSET(row + size - 1, col + size -1 + i, 240);//right bottom
				isCollision = isCollision | (background_data[pixel] ^ videoBuffer[pixel]);	
			}
		}
		if(isCollision){
			for(int i = 0; i < max; i++){//finds zombie
				if(zombies[i].health && zombies[i].row == row && zombies[i].col == col){
					zombieCollision(i);
				}
			}
			return 0;
		} else{
			return xDir;
		}

}


/**
*Returns the corrected y component direction of the object with given
*row, column, y component direction, and size. Assumes is square.
*/
int verticalCollision(int row, int col, int yDir, int size){
	int isCollision = 0;//value of the corner pixels being checked
	int pixel;//location (in offset) of pixel being checked

	if(yDir < 0){///checks above top left and right corner if move up
		//test
		for(int i = -1; i >= yDir; i--){//checks every pixel between start & end
			pixel = OFFSET(row + i, col, 240);//above topleft
			isCollision = isCollision | (background_data[pixel] ^ videoBuffer[pixel]);
			pixel = OFFSET(row + i, col + size -1, 240);//above topright
			isCollision = isCollision | (background_data[pixel] ^ videoBuffer[pixel]);
		}
	} else if(yDir > 0){//checks bottom left and right if moves down
		for(int i = 1; i <= yDir; i++){
			pixel = OFFSET(row + size - 1 + i, col, 240);//below bottom left
			isCollision = isCollision | (background_data[pixel] ^ videoBuffer[pixel]);
			pixel = OFFSET(row + size - 1 + i, col + size - 1, 240);//below bottom right
			isCollision = isCollision | (background_data[pixel] ^ videoBuffer[pixel]);
		}
	}
	if(isCollision){
			for(int i = 0; i < max; i++){//finds zombie
				if(zombies[i].health 
					&& zombies[i].row < 160 
					&& zombies[i].row > -5 
					&& zombies[i].row == row 
					&& zombies[i].col == col){
					zombieCollision(i);
				}
			}
			return 0;
		}
		else{
			return yDir;
		}
}

/**
*Moves Zombie so they don't wrap around the side
*/
void borderCheck(int i){
	if(zombies[i].isBoss && zombies[i].col + bossZSize >= 240){
		zombies[i].col = (240 - 1 - bossZSize);
		zombieCollision(i);
		zombies[i].xDir = 0;
	}
	else if(zombies[i].col + zSize >= 240){
		zombies[i].col = (240 - 1 - zSize);
		zombieCollision(i);
		zombies[i].xDir = 0;
	}

}


/**
*Kills the zombie at the given index. Resets the position to (0,0)
*Also gives player 50pts for normal kill and 200 for boss kill. 
**/
void killZombie(int index){//resets position of zombie and sets to dead.
	//clearObj(zombies[index].row, zombies[index].col, zSize);
	deadZoms[index] = 1;
	numZoms--;
	zombies[index].oldRow = zombies[index].row;
	zombies[index].oldCol = zombies[index].col;
	zombies[index].health = 0;
	zombies[index].row = 0;
	zombies[index].col = 0;
	
	if(zombies[index].isBoss){
		score += 200;
	}
	else{
		score += 50;
	}
}

/**
*Adds zombie to a spawning location (1,1)
**/
void spawnZombie(){
	//don't start next level till last zom dies
	if(levelCounter == 0 && numZoms > 0){
		return;
	}
	numZoms++;//only increment if creating zombie.
	score += numZoms;
	for(int i = 0; i < max; i++){
		if(zombies[i].health == 0){
			//Make zombies spawn randomly
			int tempY = rand() %2;
			int tempX;
			if(tempY == 1){

				tempY = -10;
				tempX = rand()%240;

			}
			else{
				tempY = 160;
				tempX = rand()%240;
			}

			//init all values 
			zombies[i].row = tempY;
			zombies[i].col = tempX;
			zombies[i].xDir = 0;
			zombies[i].yDir = 0;
			zombies[i].health = level;
			zombies[i].oldRow = zombies[i].row;
			zombies[i].oldCol = zombies[i].col;
			zombies[i].isBoss = 0;

			if(levelCounter == 1){//make last zombie of each round a boss zombie
				zombies[i].isBoss = 1;
				zombies[i].health = level * 2;
			}

			//Level up if necessary (killed last zom)
			if(levelCounter <= 0){
				zombies[i].health++;
				level++;
				levelCounter = 5 + level;
				spawnRate = 75 - level*5;//set spawnRate
				if(spawnRate < 30){ // caps spawn rate at 2 a second
					spawnRate = 30;
				}
			}
			else{
				levelCounter--;
			}
			return;
		}
	}

}


void drawZombies(){
	for(int i = 0; i < max; i++){
		int zRow = zombies[i].row;
		int zCol = zombies[i].col;
		int oldZRow = zombies[i].oldRow;
		int oldZCol = zombies[i].oldCol;
		if(zombies[i].health){
			if(zombies[i].isBoss == 0){//boss has different size
				clearObj(oldZRow, oldZCol, zSize);
				drawZombieBlock(zRow, zCol);
			}
			else{
				clearObj(oldZRow, oldZCol, bossZSize);
				drawBossZombieBlock(zRow, zCol);
			}
		}

		//removes the old image of the dead zombies.
		if(deadZoms[i]){//boss has different size
			if(zombies[i].isBoss == 0){
				clearObj(oldZRow, oldZCol, zSize);
			}
			else{
				clearObj(oldZRow, oldZCol, bossZSize);
			}
			deadZoms[i] = 0;
		}
	}
}

/**
*Checks if zombie at index z hit player
*/
void zombieCollision(int z){
	int r = zombies[z].row;
	int c = zombies[z].col;
	int x = zombies[z].xDir;
	int y = zombies[z].yDir;
	if(zombies[z].health && zombies[z].isBoss){//boss has different size
		//checks each corner to see if it hits player.
		if((r+y >= row && r+y <= row+pSize) && (c+x >= col && c+x <= col+pSize)){//top left corner
			killPlayer();
		} else if((r+bossZSize+y >= row && r+bossZSize+y <= row+pSize) && (c+x >= col && c+x <= col+pSize)){//bottom left corner
			killPlayer();
		} else if((r+y >= row && r+y <= row+pSize) && (c+bossZSize+x >= col && c+bossZSize+x <= col+pSize)){//top right
			killPlayer();
		} else if((r+y+bossZSize >= row && r+y+bossZSize <= row+pSize) && (c+x+bossZSize >= col && c+x+bossZSize <= col+pSize)){//bottom right
			killPlayer();
		} else if((r+y+bossZSize/3 >= row && r+y+bossZSize/3 <= row+pSize) && (c+x+bossZSize/2 >= col && c+x+bossZSize/2 <= col+pSize)){//checks center
			killPlayer();
		}
	}
	else if(zombies[z].health){
		//checks each corner to see if it hits player.
		if((r+y >= row && r+y <= row+pSize) && (c+x >= col && c+x <= col+pSize)){//top left corner
			killPlayer();
		} else if((r+zSize+y >= row && r+zSize+y <= row+pSize) && (c+x >= col && c+x <= col+pSize)){//bottom left corner
			killPlayer();
		} else if((r+y >= row && r+y <= row+pSize) && (c+zSize+x >= col && c+zSize+x <= col+pSize)){//top right
			killPlayer();
		} else if((r+y+zSize >= row && r+y+zSize <= row+pSize) && (c+x+zSize >= col && c+x+zSize <= col+pSize)){//bottom right
			killPlayer();
		} else if((r+y+zSize/2 >= row && r+y+zSize/2 <= row+pSize) && (c+x+zSize/2 >= col && c+x+zSize/2 <= col+pSize)){//checks center
			killPlayer();
		}
	}

}

/**
*Kills player and ends game.
**/
void killPlayer(){
	lost = 1;
	gameInProgress = 0;
}

