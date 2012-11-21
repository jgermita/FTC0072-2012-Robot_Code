

void tankDrive(int leftPower, int rightPower) {
	//Todo: add tank drive output code here
}


/*
	Drive algorithm written by Jeremy in Summer 2012. Reduces turning sensitivity without
	affecting top speed.
*/
void butterDrive(int leftPower, int rightPower) {
	float turnSensitivityScalar = .8;
	static int quickTurnThreshold = .85;

	int throttle = (leftPower + rightPower)/2;	//Convert left/right commands to throttle/turning commands
	int turning = (leftPower - rightPower)/2;

	if(abs(turning) < quickTurnThreshold) {
		turning = (int)(((float)turning)*turnSensitivityScalar);
	}

	tankDrive(throttle+turning, throttle-turning);
}

task main()
{



}
