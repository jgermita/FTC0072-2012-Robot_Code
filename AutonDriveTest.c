#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S2,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S1_C1_1,     elevator,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     left,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     right,         tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "config.c"
#include "drivetrain.c"
//#include "gyro.c"


void initializeRobot()
{
	resetEncoders();
	return;
}



task main()
{
	initializeRobot();

	waitForStart(); // Wait for the beginning of autonomous phase.

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	////                                                   ////
	////    Add your robot specific autonomous code here.  ////
	////                                                   ////
	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	int setpoint = 0;
	initDrivetrain();

	ClearTimer(T1);



	while (true)
	{
		if(joy1Btn(1)) setpoint = 0;
		if(joy1Btn(2)) setpoint = 12;
		if(joy1Btn(3)) setpoint = 24;
		if(joy1Btn(4)) setpoint = 36;

		if(joy1Btn(6)) setpoint -= joystick.joy1_y1/10;



		driveDistance(setpoint, 100);
	//	writeDebugStreamLine("angle: %f", getAngle());
	}
}
