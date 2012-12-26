#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

#define configured

//Ramp system constants
#define RAMP_CLOSED 20		//Ramp retracted servo value
#define RAMP_DEPLOYED 255	//Ramp deployed servo value
#define PLAT_CLOSED 255		//etc
#define PLAT_DEPLOYED 0
#define ENDGAME_DELAY 0	//safety timer to prevent premature deploy
#define STAGE_DELAY 375		//delay between deploying stages of ramp
