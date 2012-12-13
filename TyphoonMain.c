#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     elevator,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     left,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     right,         tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "config.c"
#ifdef configured
#include "Drivetrain.c"

void operator() {
	int liftPower = (joystick.joy2_y2*100)/128;//(joystick.joy2_y2*3)/4;
	int fineAdjust = (joystick.joy2_y1*100)/128;
  fineAdjust = (abs(fineAdjust) < 10) ? 0 : fineAdjust;
	fineAdjust /= 5;
  liftPower = (abs(liftPower) < 10) ? 0 : liftPower;
	liftPower += fineAdjust;

	if(joy2Btn(6)) {
		liftPower = -30;
		} else if(joy2Btn(8)) {
		if(time10[T1] % 10 < 5) {
			liftPower = 85;
			} else {
			liftPower = 0;
		}
	}
	int elevEnc = nMotorEncoder[elevator]*-1;
	elevEnc /= 10;
	if(joy2Btn(4)) {
		if(elevEnc < 216) {
			liftPower = 100;
		} else if(elevEnc > 225) {
			liftPower = -80;
		}
	}
	if(joy2Btn(1)) {

		if(elevEnc > 7) {
			liftPower = -100;
		}
	}
	if(joy2Btn(10)) {

		if(elevEnc < 320) {
			liftPower = 100;
		}
	}
	if(joy2Btn(2)) {
		if(elevEnc < 55) {
			liftPower = 100;
		} else if(elevEnc > 65) {
			liftPower = -80;
		}
	}
	if(joy2Btn(3)) {
		if(elevEnc < 180) {
			liftPower = 100;
		} else if(elevEnc > 190) {
			liftPower = -80;
		}
	}


	writeDebugStream("Elevator Encoder: ");
	writeDebugStreamLine("%d", elevEnc);




	motor[elevator] = liftPower;
}

task main() {
	ClearTimer(T1);
	nMotorEncoder[left] = 0;
	nMotorEncoder[right] = 0;
	nMotorEncoder[elevator] = 0;

	waitForStart();
	while(true) {
		getJoystickSettings(joystick);

		//tankDrive(joystick.joy1_y1, joystick.joy1_y2);
		operator();

		int left = (joystick.joy1_y1*100)/128;
		int right = (joystick.joy1_y2*100)/128;


		//cheesyDrive(left, right, joy1Btn(6));
		butterDrive(left, right)
		//tankDrive(left, right);
	}
}
#endif