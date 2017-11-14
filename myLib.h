#ifndef MYLIB_H
#define MYLIB_H

#include <stdio.h>
#include <stdlib.h>


#define BUTTONS *(unsigned int *)0x4000130	//memory location of buttons values
#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

//Buttons
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R 		(1<<8)
#define BUTTON_L 		(1<<9)
#define KEY_DOWN_NOW(key) 	(~(BUTTONS) & key)//returns true (non 0) if button is down 

//Graphics
#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

//Colors
#define BLACK 0x0000
#define WHITE 0x7fff
#define RED 31
#define GREEN (31<<5)
#define YELLOW (31<<5 | 31)
#define BLUE (31<< 10)
#define RGB(r, g, b) ((r) | ((g)<<5) | ((b)<<10))

//Display
#define WIDTH 239
#define HEIGHT 159
#define OFFSET(r, c, width) (((r)*(width)) + (c))

//DMA
#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// DMA channels
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

//DMA Destination Settings
#define DMA_DESTINATION_INCREMENT (0 << 21)//default
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

//DMA Source Settings
#define DMA_SOURCE_INCREMENT (0 << 23)//default
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

//Activates DMA
#define DMA_ON (1 << 31)

typedef struct OBJECT{
	int row;
	int col;
	int xDir;
	int yDir;
	int oldRow;
	int oldCol;
	int health;
	int isBoss;
} OBJECT;


typedef struct
{
	const volatile void *src;
	const volatile void *dst;
	unsigned int         cnt;
} DMA_CONTROLLER;


//main.c methods and vars
void delay(int n);
void delay(int n);
void waitForVblank();
void updateScore();
void resetVars();
extern int score;
extern int lost;
extern int timer;
extern int spawnRate;
extern int gameInProgress;
extern char scoreMsg[40];
extern char levelMsg[40];
extern unsigned short *videoBuffer;



//zombie methods and vars
void updateDirections();
void moveZombies();
void killZombie(int index);
void damageZombie(int i);
void spawnZombie();
void killPlayer();
void correctCollision();
void borderCheck(int i);
void zombieCollision(int i);
void drawZombies();
int horizontalCollision(int row, int col, int xDir, int size);
int verticalCollision(int row, int col, int xDir, int size);
extern OBJECT zombies[15];
extern const int max;
extern const int zSpeed;
extern const int zSize;
extern const int bossZSize;
extern int numZoms;
extern int level;
extern int levelCounter;


//player methods and vars 
void updatePlayer();
void movePlayer();
void shoot(int i);
void moveBullets();
void bulletHit(int index);
void drawPlayer();
void drawBullets();
void bulletCollisions();
int dirToX(int d);
int dirToY(int d);
void killBullet(int i, int size);
void debug();
void debugSpawn();
void debugKill();
void updateBullets();
extern int row;
extern int col;
extern int xDir;
extern int yDir;
extern const int playerSpeed;
extern const int pSize;
extern OBJECT bullets[15];

//graphics methods and vars
void drawBackground();
void drawMenu();
void drawZombieBlock(int row, int col);
void drawBossZombieBlock(int row, int col);
void drawSoldier(int row, int col, int dir);
void setPixel(int row, int col, unsigned short color);
void drawString(int row, int col, char str[], unsigned short color);
void drawRect(int row, int col, int height, int width, unsigned short color);
void drawRectIMG(int row, int col, int height, int width, const unsigned short *color);
void drawChar(int row, int col, char ch, unsigned short color);
void gameOver();
void drawImage(int row, int col, int width, int height, const unsigned short* image);
void clearObj(int r, int c, int size);
void drawStringFast(int row, int col, char str[]);
void drawCharFast(int row, int col, char ch);
void setStringColor(unsigned short color);
extern const unsigned short soldierN_data[49];
extern const unsigned short soldierNE_data[49];
extern const unsigned short soldierE_data[49];
extern const unsigned short soldierSE_data[49];
extern const unsigned short soldierS_data[49];
extern const unsigned short soldierSW_data[49];
extern const unsigned short soldierW_data[49];
extern const unsigned short soldierNW_data[49];
extern const unsigned short zombie_data[36]; 
extern const unsigned short zomBoss_data[64]; 
extern const unsigned char fontdata_6x8[12288];
extern unsigned short colorFont[12288];
extern const unsigned short menu_data[36000];
extern const unsigned short background_data[38400+(10*240)];
extern const unsigned short gameOver_data[36000];

#endif

