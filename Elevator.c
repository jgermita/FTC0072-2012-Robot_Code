#ifdef configured
#include "utils.h"
#define STOW 0		//Stow setpoint. this is the lowest
#define MID 215		//
#define GRAB 60
#define TOP 320

int e_setpoint = 0;
int e_error = 0;
int e_current = 0;
int openLoopPower = 0;


#define ELEVATOR_ATTEN_FACTOR 0.15
#define ELEVATOR_UP_SPEED 100
#define ELEVATOR_DN_SPEED -80

bool isOpenLoop = true;
bool isCalibrating = false;

/* Title: resetElevatorEncoder
* Parameters: none
* Returns: none
* Description: resets elevator encoder to 0 value
*/
void resetElevatorEncoder() {
	nMotorEncoder[elevator] = 0;
}

/* Title: initElevator
* Parameters: none
* Returns: none
* Description: initializes elevator systems
*/
void initElevator() {
	resetElevatorEncoder();
	motor[elevator] = 0;
	isOpenLoop = true;
	isCalibrating = false;
}

/* Title: setMotorOut
* Parameters: power: throttle for elevator motors
* Returns: none
* Description: outputs power to motors, some basic autozeroing implemented here
*/
void setMotorOut(int power) {

	if(e_current <= 0) {	//if encoder is negative, reset the encoder
		resetElevatorEncoder();
	}

	if(abs(power) < 10) {	//Deadband to prevent motor from drifting
		power = 0;
	}

	motor[elevator] = power;
}


/* Title: calibrateElevator
* Parameters: none
* Returns: none
* Description: runs the elevator motors up to unjam cables, and down all the way to hit lower mechanical limit
* 							and resets encoders
*/
void calibrateElevator() {
	ClearTimer(T2);
	while(time1[T2] <= 300) {	//Drive elevator up a little bit to unjam
		motor[elevator] = 100;
	}

	ClearTimer(T2);
	while(time1[T2] <= 1500) {	//Drive elevator down for a few seconds to find true zero
		motor[elevator] = -100;
	}

	motor[elevator] = 0;
	resetElevatorEncoder();
}

/* Title: openLoopInput
* Parameters: power: raw motor power
* Returns: none
* Description: puts elevator system into open loop mode and allows for drivers to tweak height
*/
void openLoopInput(int power) {

	openLoopPower = power;
}

/* Title: closedLoopInput
* Parameters: setpoint: the height to set the elevator to, enabled: boolean input to enable closed loop control
* Returns: none
* Description: puts elevator into closed loop mode and sets the positioning loop to go to the specified height
*/
void closedLoopInput(int setpoint, bool enabled) {

	e_setpoint = setpoint;
}

/* Title: elevatorControlLoop
* Parameters: none
* Returns: none
* Description: elevator positioning loop. Contains two different control algorithms:
*							-BangBang - simplest control loop. sets constant power to try to go to the setpoint
*							-Exponential decay - uses an exponential function to drive full power until near the setpoint,
where it acts as a nonlinear proportional loop
*/
void elevatorControlLoop() {
	e_error = e_setpoint - e_current;
	float output = 0;

	if(abs(e_error) < 10) {	//Error deadband to allow for a slight amount of error.
		e_error = 0;
	}

	//Exponential decay control:
	output = (100*(1-pow(2.718, -ELEVATOR_ATTEN_FACTOR*abs(e_error))))*signum(e_error);

	//BangBang Control:
	//if(e_error > 0) {
	//	output = ELEVATOR_UP_SPEED;	//Up and down speed are different to be a bit smoother
	//} else if(e_error <
	//	0) {
	//	output = ELEVATOR_DN_SPEED;
	//} else {
	//	output = 0;
	//}

	setMotorOut((int) output);
}

task ElevatorControlTask() {
	initElevator();

	while(true) {
		e_current = nMotorEncoder[elevator]/-10;
		if(!isOpenLoop) {
			elevatorControlLoop();
		} else if(isCalibrating) {
			PlaySound(soundBeepBeep);
			calibrateElevator();
			isCalibrating = false;
		} else {
			setMotorOut(openLoopPower);
		}
		wait1Msec(5);
	}
}
#else
#error "Motor and sensor configuration file not included!"
#endif
