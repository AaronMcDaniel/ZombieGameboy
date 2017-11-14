#include "myLib.h"

//pointer for drawing soldier. Changes based on direction. 
const unsigned short *soldier;


/**
*Sets the pixel on the screen at the given row and column to the given color.
*/
void setPixel(int row, int col, unsigned short color){
	//set the pixel
	unsigned int offset = OFFSET(row, col, 240);
	videoBuffer[offset] = color;
}

/**
*Draws character ch at the given row and col.
*Draws it in the given color.
*/
void drawChar(int row, int col, char ch, unsigned short color)
{
	int r;
	int c;
	const unsigned char *font = fontdata_6x8;
	for(r = 0; r < 8; r++)
	{
		for(c = 0; c < 6; c++)
		{
			if (font[OFFSET(r, c, 6) + ch*48])
			{
				setPixel(row + r, col + c, color);
			}
		}
	}
}

/**
*Draws char using DMA.
*To choose color of words use setStringColor. Otherwise will be black.
*/
void drawCharFast(int row, int col, char ch){
	int r;
	int offset;
	for(r =0; r < 8; r++){
		offset = OFFSET(r, 0, 6) + ch*48;
		DMA[3].src = colorFont + offset;
		DMA[3].dst = videoBuffer + OFFSET(row+r, col, 240);
		DMA[3].cnt = 6 | DMA_ON;

	}
}

/**
*Draws every char in given string until finds null character. 
*Draws the chars in the given color starting at the given Row and Column.
*/
void drawString(int row, int col, char str[], unsigned short color){
	while(*str)
	{
		drawChar(row, col, *str++, color);
		col += 6;
		
	}
}

/**
*Draws every char in the given string until finds null character.
*Draws chars using DMA method. Must set color before using this method.
*/
void drawStringFast(int row, int col, char str[]){
	while(*str)
	{
		drawCharFast(row, col, *str++);
		col += 6;
	}
}

/**
*Updates short[] copy of fontdata_6x8, but instead of 1's they are the given color.
*Instead of 0's they are a random color from the background image, so the letters blend in.
*/
void setStringColor(unsigned short color){
	int r;
	int c;
	unsigned int i;
	for(i = 0; i < 12288; i++){
		if(fontdata_6x8[i]){
			colorFont[i] = color;
		}
		else{
			//chooses color from 0 to 31
			colorFont[i] = background_data[(i<<27)>>27];
		}
	}
}

/**
*Draws the start menu.
*Draws without DMA because it looks cool.
*/
void drawMenu(){
	int r;
	int c;
	for(r=0; r< 150; r++)
	{
		for(c=0; c<240; c++)
		{
			setPixel(r, c, menu_data[OFFSET(r, c, 240)]);
		}
	}
}

/**
*Draws the game over screen.
*Doesn't use DMA because it is cool.
*/
void gameOver(){
	static char first = 1;
	volatile int delay = 800;
	while(delay > 0){//for dramatic effect.
		waitForVblank();
		delay--;
	}

	if(!first){
		//draws monkey image without DMA because it looks cool
		int r;
		int c;
		for(r=0; r< 150; r++)
		{
			for(c=0; c<240; c++)
			{
				setPixel(r, c, gameOver_data[OFFSET(r, c, 240)]);
			}
		}
	} 
	else{//draws monkey image with DMA because grades
		first = 0;
		drawImage(0, 0, 240, 150, gameOver_data);
	}

	//blacks out bottom rows so can display score.
	drawRect(150, 0, 240, 10, BLACK);
	sprintf(scoreMsg, "Score: %d   Level: %d", score, level);//update message format.
	drawString(151, 5, scoreMsg, RGB(20, 5, 5));//draws slow because background black.

	//Writes game over on gorrilla's forehead.
	char over[10] = "Game Over";
	drawString(20, 93, over, GREEN);
}

/**
*Draws the background image.
*Draws on extra rows above and below the screen so zombies can spawn off screen.
*/
void drawBackground(){
	drawImage(-10, 0, 240, 10, (background_data - 10*240));
	drawImage(0, 0, 240, 160, background_data);
	drawImage(160, 0, 240, 10, (background_data + 160*240));
}

/**
*Draws soldier picture at given location.
*Draws sprite corresponding to given direction.
*/
void drawSoldier(int row, int col, int dir){
	if(dir == 0){
		soldier = soldierN_data;
	} else if(dir == 1){
		soldier = soldierNE_data;
	} else if(dir == 2){
		soldier = soldierE_data;
	} else if(dir == 3){
		soldier = soldierSE_data;
	} else if(dir == 4){
		soldier = soldierS_data;
	} else if(dir == 5){
		soldier = soldierSW_data;
	} else if(dir == 6){
		soldier = soldierW_data;
	} else {
		soldier = soldierNW_data;
	}
	drawImage(row, col, pSize, pSize, soldier);
}

/**
*Draws zombie image at given location.
*/
void drawZombieBlock(int row, int col){
	drawImage(row, col, zSize, zSize, zombie_data);
}

/**
*
*/
void drawBossZombieBlock(int row, int col){
	drawImage(row, col, bossZSize, bossZSize, zomBoss_data);
}



/**
*Draws given color rectangle using DMA.
*Puts rectangle at given row and column.
*/
void drawRect(int row, int col, int width, int height, unsigned short color) {
    DMA[3].src = &color;
    int r;
    for (r = 0; r < height; r++) {
        DMA[3].dst = videoBuffer + OFFSET((row + r), col, 240);
        DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
    }
}

/**
*Draws rectangle that is segment of given image at the corresponding row and column.
*Rectangle is placed at given row and column on screen. Uses DMA.
*Used to clear old images without destroying background image.
*/
void drawRectIMG(int row, int col, int width, int height, const unsigned short *img) {
	int offset;
    int r;
	
    for (r = 0; r < height; r++) {
    	offset = OFFSET((row + r), col, 240);
    	DMA[3].src = img + offset;
        DMA[3].dst = videoBuffer + offset;
        DMA[3].cnt = width | DMA_ON;
    }
}

/**
*Draws the given image at the given row and column.
*Uses DMA.
*/
void drawImage(int row, int col, int width, int height, const unsigned short* image) {
	int r;
	
    for (r = 0; r < height; r++) {
        DMA[3].src = image + (width*r);
        DMA[3].dst = videoBuffer + OFFSET((row + r), col, 240);
        DMA[3].cnt = width | DMA_ON;

    }
}

/*
*Clears object of given size at given location by drawing background over it.
*/
void clearObj(int row, int col, int size){
	drawRectIMG(row, col, size, size, background_data);
}
