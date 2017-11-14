#include "myLib.h"

//All vars used in multiple files. Used this so I know which ones
//may be changed in mothods from other files.


//From Zombie
OBJECT zombies[15];
int numZoms;
int level = 1;
int levelCounter = 5;
const int max = 15; 
const int zSpeed = 1; 
const int zSize = 8;
const int bossZSize = 14;


//From Player
OBJECT bullets[15];
int row = 80; //row that player is in
int col = 80; //col player is in. defines spawn point
int xDir =0;
int yDir =0;
const int pSize = 7;
const int playerSpeed = 2; //player should be faster than zombies

//from main
unsigned short *videoBuffer = (unsigned short *)0x6000000;
int timer;
int score;
int spawnRate = 70;
char scoreMsg[40];
char levelMsg[40];
int gameInProgress = 0;
int lost = 0;



