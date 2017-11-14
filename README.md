# ZombieGameboy

This game was created by Aaron McDaniel. I did not write the make files, just the c code and headers.

To install the Virtual GameBoy emulator using the following commands:

sudo​ apt-get​ install​ gcc-arm-none-eabi
sudo​ apt-get​ install​ libnewlib-arm-none-eabi
sudo​ apt-get​ install​ cs2110-vbam-sd

To run the game use the command: 

make vba

If you use an OS other than ubantu, you can run the .gba file in the GBA folder using a gameboy emulator. 


CONCEPT
***************************************
This is a Gameboy version of the Nazi Zombies mini game called "Dead Ops Arcade."
The Start menu, game over screen, and background images are all from the original game.

RULES
***************************************
Your goal is to avoid the zombies at all costs and kill as many as you can.
You only have one life, so live it well.
If a zombie touches you, then the game will end.
To avoid getting killed by zombies you can either run away or shoot them to kill them.
You have a sniper and a machine gun. Try using those.

WEAPONS
**************************************
Sniper -> Semi automatic weapon with stopping power of 3. Can hit up to 3 zombies in one shot.
Machine Gun -> Automatic Weapon. Can only Hit 1 zombie at a time. Fire rate increases each level until level 8.
To make it easy to tell the difference, Sniper bullets are red until they have stopping power of 1, and Machine gun bullets are white.

CONTROLS
***************************************
D-Pad -> move (can move diagonally)
A Button -> Shoot sniper (semi-automatic)
B Button -> Shoot machine gun (automatic)
Left trigger & Right trigger -> Aim mode (D-Pad will change only your direction, while holding L or R)
Start Button -> Pause (in game) Next Screen/Start Game (Out of game)
Select Button -> Return to main menu.

LEVELS
***************************************
The zombies spawn in waves, and each wave will start when you kill the last zombie of the previous wave, or you start the game.
Each wave is more difficult than the last.
There is no final wave. You will eventually die.
The health of the zombies increases each wave. On level 1 it takes 1 shot to kill a zombie. On level 2 it takes 2 shots, and so on.
Each wave also contains 1 more zombie than the wave before it. However there will never be more than 15 zombies on the screen at a time.
The last zombie of each wave is a boss. The boss has double health and is larger.

SCORE
***************************************
Each time a zombie spawns, your get one point for every zombie on the screen.
Each time you shoot a zombie or boss zombie you get 10 points.
You get 50 points for killing a zombie and 200 for killing a boss zombie.

STRATEGY
***************************************
Try using your speed advantage to run circles around the zombies. They will eventually group together.
Use your Sniper to kill zombies when they are grouped together.
Use your machine gun to kill individual zombies, or kill a specific zombie faster.
Use the aim buttons to shoot in different directions without moveing toward the zombies.
