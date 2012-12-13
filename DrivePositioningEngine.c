int x = 0;
int y = 0;
int targX = 0;
int targY = 0;

int leftOut = 0;
int rightOut = 0;

void init() {
	resetEncoders();
}

void resetEncoders() {
	nMotorEncoder[left] = 0;
	nMotorEncoder[right] = 0;
}

void update() {
	int distTraveled = (nMotorEncoder[left] + nMotorEncoder[right])/2;

	int heading = 0;	//TODO: Change this 0 to a gyro value
										//TODO: Convert heading to radians if nto already

	x += distTraveled*cos(heading);
	y += distTraveled*sin(heading);

	resetEncoders();


}

int getLeftOutput() {
	return leftOut;
}

int getRightOutput() {
	return rightOut;
}
