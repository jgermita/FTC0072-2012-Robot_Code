#include "utils.h"

#define STOW 0
#define MID 216
#define GRAB 60

void setMotorOut(int power) {
	bool topLimit = false;
	bool bottomLimit = false;

	if(topLimit && signum(power) == 1) {
		power = 0;
		//todo: set encoder max
	}

	if(bottomLimit && signum(power) == -1) {
		power = 0;
		//todo: zero encoder
	}

	//todo: output to motors
}

int e_setpoint = 0;
int e_error = 0;
int e_current = 0;
int maxSpeed = 75;
float attenuationFactor = 1.0;

void elevatorControlLoop() {
	e_current = 0;
	e_error = e_setpoint - e_current;

	float output = (maxSpeed*(1-pow(2.718, -attenuationFactor*abs(e_error))))*signum(e_error);

	setMotorOut((int) output);

}
