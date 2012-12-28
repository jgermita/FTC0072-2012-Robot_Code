#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#define configured
#include "Drivetrain.c"
#include "elevator.c"
#include "SignalLightStrip.c"

//Controls mappings
#define STOW_BTN 1
#define GRAB_BTN 2
#define MID_BTN 3
#define TOP_BTN 4

//Ramp system constants
#define RAMP_CLOSED 20		//Ramp retracted servo value
#define RAMP_DEPLOYED 255	//Ramp deployed servo value
#define PLAT_CLOSED 255		//etc
#define PLAT_DEPLOYED 0
#define ENDGAME_DELAY 0	//safety timer to prevent premature deploy
#define STAGE_DELAY 375		//delay between deploying stages of ramp

void initRobot() {
	servo[rampLatch] = 20;
	servo[platLatch] = 255;
	ClearTimer(T1);	 //Clear timer 1 for checking to see how long the robot waits before starting
	initDrivetrain();//Initialize systems
	initElevator();
	StartTask(ElevatorControlTask);	//begin elevator control task
	StartTask(SignalLight);
	setMode(IDLE);
}
