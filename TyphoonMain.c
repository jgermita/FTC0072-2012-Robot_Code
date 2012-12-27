#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S3,     HTPB,                sensorI2CCustom9V)
#pragma config(Motor,  mtr_S1_C2_1,     elevator,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     left,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     right,         tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    rampLatch,            tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    platLatch,            tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

float timeEnabled = 0;
float deltaT = 0;
#include "config.c"
#ifdef configured
#include "Drivetrain.c"
#include "elevator.c"
#include "SignalLightStrip.c"


#define STOW_BTN 1
#define GRAB_BTN 2
#define MID_BTN 3
#define TOP_BTN 4


/* Title: operator
* Parameters: none
* Returns: none
* Description: encloses operator commands in one function for easy tweaking
*/
void operator() {
	int rawJoystick = 0;														//Open Loop Joystick input
	rawJoystick += (joystick.joy2_y2*100)/128;
	rawJoystick += ((joystick.joy2_y1*100)/128)/3;

	if(joy2Btn(STOW_BTN)) {				//Logic for choosing setpoints or open loop control
		isOpenLoop = false;
		closedLoopInput(STOW, true);
	} else if(joy2Btn(GRAB_BTN)) {
	isOpenLoop = false;
		closedLoopInput(GRAB, true);
	} else if(joy2Btn(MID_BTN)) {
	isOpenLoop = false;
		closedLoopInput(MID, true);
	} else if(joy2Btn(TOP_BTN)) {
	isOpenLoop = false;
		closedLoopInput(TOP, true);
	} else {
	isOpenLoop = true;
	openLoopInput(rawJoystick);
	}

	if(joy2Btn(9) && joy2Btn(10)) {
		isCalibrating = true;
	}
}

/* Title: main
* Parameters: none
* Returns: none
* Description: main task. initiates auxillary tasks and contains driver inputs
*/
task main() {
	servo[rampLatch] = 20;

	servo[platLatch] = 255;
	ClearTimer(T1);	 //Clear timer 1 for checking to see how long the robot waits before starting
	initDrivetrain();//Initialize systems
	initElevator();
	StartTask(ElevatorControlTask);	//begin elevator control task
	StartTask(SignalLight);
	setMode(IDLE);

	waitForStart(); //Wait for FCS start

	if(time1[T1] >= 60000)  {	//if robot has been sitting for a while before startin, call init again to zero and reset gyro and encoders
		initDrivetrain();
		initElevator();
	}

	ClearTimer(T1);	//Clear Timer 1 for robot functions before operation starts
	while(true) {
		deltaT = time1[T1] - timeEnabled;
		timeEnabled = time1[T1]/1000.0;

		getJoystickSettings(joystick);	//Get FCS controller data
		//operator();	//Run operator commands

		int left = (joystick.joy1_y1*100)/128;		//Driver inputs, scaled to the motor controller input range of +- 100
		int right = (joystick.joy1_y2*100)/128;

		int throttle = (joystick.joy1_y1*100)/128;		//Driver inputs, scaled to the motor controller input range of +- 100
		int turn = (joystick.joy1_x2*100)/128;
		linearTankDrive(throttle + turn, throttle - turn);

		if(timeEnabled >= 110) {
			setMode(SUPER_CRITICAL);
		} else if(timeEnabled >= 75) {
			setMode(CRITICAL);
		} else {
			setMode(ACTIVE);
		}

		//writeDebugStreamLine("Sampling Started...");
		////PlaySound(soundBeepBeep);
		//for(int i = 0; i <= 100; i++) {
		//deltaT = time1[T1] - timeEnabled;
		//timeEnabled = time1[T1]/1000.0;

		//	for(int j = 0; j <= 10; j++) {
		//				deltaT = time1[T1] - timeEnabled;
		//timeEnabled = time1[T1]/1000.0;

		//		tankDrive(0, i);
		//		wait1Msec(80);
		//	}
		//}

		//PlaySound(soundBeepBeep);
		//writeDebugStreamLine("Sampling done");
		//break;
		////tankDrive(throttle + turn, throttle - turn);

		//if(joy1Btn(5) && joy1Btn(6) && timeEnabled > ENDGAME_DELAY) {
		//	servo[rampLatch] = RAMP_DEPLOYED;
		//	wait1Msec(STAGE_DELAY);
		//	servo[platLatch] = PLAT_DEPLOYED;
		////	PlaySound(soundFastUpwardTones);
		//	} else {
		//	servo[rampLatch] = RAMP_CLOSED;
		//	servo[platLatch] = PLAT_CLOSED;
		//}

		//writeDebugStream("Enabled for %f", timeEnabled);	//Enabled timer
		//writeDebugStreamLine(" seconds");
		wait1Msec(1);
	}
}
#endif
