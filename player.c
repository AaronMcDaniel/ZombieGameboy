#include "myLib.h"

//static vars
//OBJECT player;

static const int bulletSpeed = 6;
static int oldCol = 80;
static int oldRow = 80;
static int deadBullets[15];//boolean list of if bullet died this frame
int dir = 0; //direction of player. numbers 0-7 represent directions possible to face

/**
*Moves player, also fires if player presses A or B
**/
void updatePlayer(){
	movePlayer();
	static int fired = 0;
	static int fireRate = 10;

	//hold A & B defaults to Auto
	//A for semi automatic powerful fire
	if(KEY_DOWN_NOW(BUTTON_A) && !KEY_DOWN_NOW(BUTTON_B) && fired == 0){
		shoot(3);
		fired = 1;
	} else if(!KEY_DOWN_NOW(BUTTON_A)){
		fired = 0;
	}

	//B for automatic fire, rate of fire increases with level. 
	if(KEY_DOWN_NOW(BUTTON_B) && fireRate <= 0){
		shoot(1);
		fireRate = 10 - level;
		if(fireRate < 2){// lowest can get is 5 frames per shot
			fireRate = 5;
		}
	} else if(fireRate > 0){//decrements fireRate until 0
		fireRate --;
	}

	
	//if(KEY_DOWN_NOW(BUTTON_R)){
	//	debug();
	//}
}

/**
*Moves bullets and corrects fir collision
*/
void updateBullets(){
	moveBullets();
	bulletCollisions();
}

/**
*Moves player based on user input. Take into account screen borders.
**/
void movePlayer(){
	xDir = 0;
	yDir = 0;
	

	//sets direction and component speed of player
	if(KEY_DOWN_NOW(BUTTON_DOWN)){//S
		yDir = playerSpeed;
		dir = 4;
		if(KEY_DOWN_NOW(BUTTON_LEFT)){//SE
			xDir = -playerSpeed;
			dir = 5;
		} else if(KEY_DOWN_NOW(BUTTON_RIGHT)){//SW
			xDir = playerSpeed;
			dir = 3;
		} 
	} else if(KEY_DOWN_NOW(BUTTON_UP)){//N
		yDir = -playerSpeed;
		dir = 0;
		if(KEY_DOWN_NOW(BUTTON_LEFT)){//NE
			xDir = -playerSpeed;
			dir = 7;
		} else if(KEY_DOWN_NOW(BUTTON_RIGHT)){//NM
			xDir = playerSpeed;
			dir = 1;
		}
	} else{//not up or down pressed
		if(KEY_DOWN_NOW(BUTTON_LEFT)){//E
			xDir = -playerSpeed;
			dir = 6;
		} else if(KEY_DOWN_NOW(BUTTON_RIGHT)){//W
			xDir = playerSpeed;
			dir = 2;
		}
	}

	//locks position for aiming in place
	if(KEY_DOWN_NOW(BUTTON_R) || KEY_DOWN_NOW(BUTTON_L)){
		xDir = 0;
		yDir = 0;
	}

	//xDir = horizontalCollision(row, col, xDir, pSize);
	//yDir = verticalCollision(row, col, yDir, pSize);
	
		//collision with border
		if(row + yDir < 10){//collide with top
			row = 10;
			yDir = 0;
		} else if((row + yDir + pSize) > HEIGHT){//collide with bottom;
			row = (HEIGHT-pSize + 1);
			yDir = 0;
		} else{
			row += yDir;
		}
		if(col + xDir < 0){//collide with left
			col = 0;
			xDir = 0;
		} else if(col + xDir + pSize > WIDTH){//collide with right
			col = WIDTH - pSize + 1;
			xDir = 0;
		} else{
			col += xDir;
		}	
}

/**
*Shoots a bullet going in the direction the player is facing
*Negative healths will not die on impact of zombies
**/
void shoot(int h){
	for(int i = 0; i < max; i++){
		//finds a dead bullet and makes it alive
		if(bullets[i].health == 0){
			bullets[i].health = h;

			bullets[i].xDir = dirToX(dir)*bulletSpeed;
			bullets[i].yDir = dirToY(dir)*bulletSpeed;

			//bullets spawn at center of player
			bullets[i].row = row + 3;
			bullets[i].col = col + 3;
			return;
		}
	} 
}

/**
*Moves all of the bullets
**/
void moveBullets(){
	//Moves bullets. Calls killZombie if is a hit
	OBJECT *ptr = bullets;
	for(int i =0; i < max; i++){
		//only moves bullet if has health
		if(ptr->health){
			//kills the bullet if it moves off screen

			if(ptr->row + yDir < -1 
				|| ptr->col + ptr->xDir < 0 
				|| ptr->col + ptr->xDir > 238 
				|| ptr->row + ptr->yDir > 159){//if about to go off screen
				deadBullets[i] = 1;
				ptr->health = 0;
				ptr->oldRow = ptr->row;
				ptr->oldCol = ptr->col;
			}
			else{
				ptr->oldRow = ptr->row;
				ptr->oldCol = ptr->col;
				ptr->row += ptr->yDir;
				ptr->col += ptr->xDir;
			}
		}
		ptr++;
	}
}

/**
*Check if bullet hits any zombie. If so, it kills bullet and Zombie giving player points.
*/
void bulletCollisions(){//checks for bullet collision with bullet at index b
	int b;
	OBJECT* zom;
	//loops through all bullets
	for(b = 0; b < max; b++){
		zom = zombies;
		if(bullets[b].health){
			int r = bullets[b].row;
			int c = bullets[b].col;
			int y = bullets[b].yDir;
			int x = bullets[b].xDir;
			if(horizontalCollision(r, c, x, 2) == 0
			 || verticalCollision(r, c, y, 2) == 0){//if collides check zombies
				//make x and y equal to 1 in the same direction or 0 (makes unit vector)
				/*if(y > 0){
					y = 1;
				} else if(y < 0){
					y = -1;
				}
				if(x > 0){
					x = 1;
				} else if(x < 0){
					x = -1;
				}*/
				//checks bullet b against all zombies
				for(int i = 0; i < max; i++){
					if(zom -> health && zom -> isBoss){//for boss
						//has to check intermediate pixels it went through (so doesn't teleport past zombies)
						//for(int s = bulletSpeed - 1; s >= 0; s -= 4){//start closest to oldRow and oldCol (so hits closest BossZombie)
							if(r >= zom->row && r <= (zom->row + bossZSize)//if bullet is between top and bottom of BossZombie
								&& c >= zom->col && c <= zom->col+bossZSize){//if bullet is between left and right of BossZombie
								damageZombie(i);
								killBullet(b, bossZSize);
								return;//only hits one Zombie at a time
							}
						//}
					}
					else if(zom -> health){//for normal zombies
						//has to check intermediate pixels it went through (so doesn't teleport past zombies)
						//for(int s = bulletSpeed - 1; s >= 0; s -= 4){//start closest to oldRow and oldCol (so hits closest Zombie)
							if(r  >= zom->row && r <= (zom->row + zSize)//if bullet is between top and bottom of Zombie
								&& c >= zom->col && c <= zom->col+zSize){//if bullet is between left and right of Zombie
								damageZombie(i);
								killBullet(b, zSize);
								return;//only hits one Zombie at a time
							}
						//}
					}
					zom++;
				}
			}
		}
	}
}

/**
*Damages zombie health. Gives 10 points every time player damages Zombie
*Kills zombie if health gets below 0.
*/
void damageZombie(int i){
	zombies[i].health--;
	score += 10;
	if(zombies[i].health <= 0){
		killZombie(i);
	}
}


/**
*Draws player and blacks out the old image
**/
void drawPlayer(){
	clearObj(oldRow, oldCol, pSize);
	drawSoldier(row, col, dir);

	oldCol = col;
	oldRow = row;
}

/**
*Draws all bullets in their new locations and clears the previous image.
*/
void drawBullets(){
	for(int i = 0; i < max; i++){
		if(bullets[i].health == 1){//normal bullets are White
			clearObj(bullets[i].oldRow, bullets[i].oldCol, 2);
			drawRect(bullets[i].row, bullets[i].col, 2, 2, WHITE);
		}
		else if(bullets[i].health > 1){//sniper bullets are Red
			clearObj(bullets[i].oldRow, bullets[i].oldCol, 2);
			drawRect(bullets[i].row, bullets[i].col, 2, 2, RED);
		}
		if(deadBullets[i]){//if just died, clear.
			deadBullets[i] = 0;
			clearObj(bullets[i].oldRow, bullets[i].oldCol, 2);
		}
	}
}

/**
*Decrements the health of the bullet at i.
*If bullet i's health goes to 0 then bullet dies, else moves past current enemy.
*Size is the size of the enemy it hit.
*/
void killBullet(int i, int size){
	bullets[i].health --;
	if(bullets[i].health){ // keeps going
		int x = bullets[i].xDir;
		int y = bullets[i].yDir;
		//make x and y equal to 1 in the same direction or 0 (makes unit vector)
		if(y > 0){
			y = 1;
		} else if(y < 0){
			y = -1;
		}
		if(x > 0){
			x = 1;
		} else if(x < 0){
			x = -1;
		}
		bullets[i].col += (size)*x;// + 1 to move past zombie.
		bullets[i].row += (size)*y;//only double damage on boss.
	}
	else {//dies
		deadBullets[i] = 1;
	}

}

/**
*Takes in the direction (0:7).
*Outputs the corresponding X component direction.
*/
int dirToX(int d){
	if(d > 0 && d < 4){
		return 1;
	}
	else if(d > 4 && d < 8){
		return -1;
	}
	else{
		return 0;
	}
}

/**
*Tales in the direction (0:7).
*Outputs corresponding Y component direction.
*/
int dirToY(int d){
	if(d > 2 && d < 6){
		return 1;
	} 
	else if(d < 2 || d > 6){
		return -1;
	}
	else{
		return 0;
	}
}

/**
*Puts game in 'debug' mode, used for debugging.
*/
void debug(){

	debugSpawn();
	if(KEY_DOWN_NOW(BUTTON_L)){
		debugKill();
	}
	if(KEY_DOWN_NOW(BUTTON_R)){
		killPlayer();
	}
}

/**
*Puts all zombies in a line to keep track of live zombies.
*Also displays which zombies are alive and dead at top left corner.
*/
void debugSpawn(){
	OBJECT *zom = zombies;
	for(int i = 0; i < max; i++){
		if(zom->health){
			clearObj(zom->row, zom->col, zSize);//erases old image
			zom->row = 51;
			zom->col = zSize*i;
			zom->xDir = 0;
			zom->yDir = 0;
			setPixel(0, (10+(2*i)),GREEN);//Green pixel is live zombie
		} else{
			setPixel(0, (10+(2*i)), BLACK);//Black pixel is dead zombie
		}
		setPixel(0, (11+(2*i)),YELLOW);//Seperates live/dead pixels for visibility
		zom++;
	}
}

/**
*Resets score and kills all zombies.
*/
void debugKill(){
	score = 0;
	for(int i = 0; i < max; i++ ){
		if(zombies[i].health){
			killZombie(i);
		}
		bullets[i].health = 0;
	}
}





