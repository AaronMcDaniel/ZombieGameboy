#define BUTTONS *(unsigned int *)0x4000130	//memory location of buttons values
#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

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



#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define BLACK 0x0000
#define WHITE 0x8fff
#define RED 31
#define GREEN (31<<5)
#define YELLOW (31<<5 | 31)
#define BLUE (31<< 10)

#define WIDTH 239
#define HEIGHT 159
//calcs offset in array to get location at (r,c) with a width of width
#define OFFSET(r, c, width) ((r*width) + c)

//extern int row;
//extern int col;
extern const unsigned char fontdata_6x8[12288];
unsigned short *videoBuffer = (unsigned short *)0x6000000;

//zombie
void updateZombies();
void updateDirections();
void moveZombies();
void killZombie(int index);
void spawnZombie();
void killPlayer();
void correctCollision();
void drawZombies();
extern int zombies[6*50];

//player
void updatePlayer();
void movePlayer();
void shoot();
void moveBullets();
void bulletHit(int index);
void drawPlayer();
extern int row;
extern int col;
extern const int playerSpeed; //player should be faster than zombies
extern const int pSize;