#ifdef configured
#include "gyro.c"
int leftEnc = 0;
int prevLeft = 0;
int rightEnc = 0;
int prevRight = 0;

float leftVel = 0;
float rightVel =0;

int countsToInches(int counts) {
	int countsPerRotation = 720;
	counts *= (countsPerRotation);
	counts /= 3.0*PI;
	return counts;
}


/* Title: resetEncoders
 * Parameters: none
 * Returns: none
 * Description: Resets encoders on drivetrain to zero values
 */
void resetEncoders() {
	nMotorEncoder[left] = 0;
	nMotorEncoder[right] = 0;
}


/* Title: initDrivetrain
 * Parameters: none
 * Returns: none
 * Description: Initializes drivetrain by resetting encoders and starting the gyro task
 */
void initDrivetrain() {
	resetEncoders();
	StartTask(gyroTask);
	writeDebugStreamLine("Drivetrain initialized!");
}


/* Title: tankDrive
 * Parameters: leftPower: left drive input, rightPower: right drive input
 * Returns: none
 * Description: processes and outputs drive commands to motor controllers
 */
void tankDrive(int leftPower, int rightPower) {
	//leftPower = (abs(leftPower) < 10) ? 0 : leftPower;
	//rightPower = (abs(rightPower) < 10) ? 0 : rightPower;

	prevRight = rightEnc;
	rightEnc = nMotorEncoder[right]*-1;

	if(rightEnc != prevright) {
		rightVel = rightEnc - prevright;
		rightVel /=deltaT;
		rightVel *= 1000.0;
	}

	writeDebugStreamLine("Vel: %f", rightVel);
	//rightEnc = nMotorEncoder[right]*-1;

	//rightPower = (int)((float)rightPower*.875);

	motor[left] = leftPower;
	motor[right] = -rightPower;
}

void inverseSquareTankDrive(float leftPower, float rightPower) {
	float outputL = 12*(sqrt(abs(leftPower/2)))*sgn(leftPower);
	float outputR = 12*(sqrt(abs(rightPower/2)))*sgn(rightPower);

	//if(abs(leftPower) < 30) {
	//	outputL = leftPower;
	//}
	//if(abs(rightPower) < 30) {
	//	outputR = rightPower;
	//}

	if(abs(outputL) > 100) outputL = 100*sgn(outputL);
	if(abs(outputR) > 100) outputR = 100*sgn(outputR);
	writeDebugStreamLine("LeftOutput: %f", outputL);
	tankDrive((int)outputL, (int) outputR);
}


//void butterDrive(int leftPower, int rightPower) {
//	float turnSensitivityScalar = .7875;
//	int quickTurnThreshold = 60;

//	int throttle = (leftPower + rightPower)/2;	//Convert left/right commands to throttle/turning commands
//	int turning = (leftPower - rightPower)/2;

//	if(abs(turning) < quickTurnThreshold) {
//		turning = (int)(((float)turning)*turnSensitivityScalar);
//	}

//	tankDrive(throttle+turning, throttle-turning);
//}

//float tSens = 1.5;               //Cheesy drive turning sensitivity scalar
//float TURBO_MODE_TSENS = 2;      //Turbo mode turning sensitivity
//float NORMAL_MODE_TSENS = 1.5;   //Normal(slow) mode turning sensitivity
//float NORMAL_MODE_SCALAR = .8;   //Scalar for making normal mode slower than turbo mode

//bool isTurboMode = true;             //Defaults to turbo mode. Turbo mode is full speed

//void cheesyDrive(int throttle, int wheel, bool quickTurn) {

//	//throttle = (float)((float)throttle)/100.0;
//	//wheel = (float)((float)wheel)/100.0;

//	int angular_power = 0;
//	int overPower = 0;
//	float sensitivity = tSens;
//	int rPower = 0;
//	int lPower = 0;

//	if(quickTurn) {
//		overPower = 100;
//		sensitivity = 1.0;
//		angular_power = wheel;
//	}
//	else {
//		overPower = 0;
//		angular_power = (int)((float)((abs(throttle) * wheel)/10000) * sensitivity);
//	}

//	rPower = lPower = throttle;
//	lPower += angular_power;
//	rPower -= angular_power;

//	if(lPower > 100) {
//		rPower -= overPower * (lPower - 100);
//		lPower = 100;
//	}
//	else if(rPower > 1.0) {
//		lPower -= overPower * (rPower - 100);
//		rPower = 100;
//	}
//	else if(lPower < -100) {
//		rPower += overPower * (-100 - lPower);
//		lPower = -100;
//	}
//	else if(rPower < -100) {
//		lPower += overPower * (-100 - rPower);
//		rPower = -100;
//	}

//	tankDrive(lPower, rPower);
//}


/* Title: distanceExpDec
 * Parameters: distance: distance error input, maxSpeed: maximum allowable speed for output
 * Returns: float output of Exponential decay control loop
 * Description: processes a distance error and max speed input to a motor output for positioning
 *   						control of drive.
 */
float distanceExpDec(int distance, int maxSpeed) {
	float	distAttenuation = 0.01;
	float answer = 1.0;
	distAttenuation *= -abs(distance);
	answer -= pow(2.718, distAttenuation);
	answer *= (float) maxSpeed*sgn(distance);
	return answer;
}


/* Title: driveDistance
 * Parameters: inches: specify distance to drive in inches, maxSpeed: top speed allowed for driving
 * Returns: none
 * Description: controls drivetrain positioning in the Y(forward/backward) translational axis
 *							precise to less than 1/4"
 */
void driveDistance(int inches, int maxSpeed) {
	int countsPerRotation = 720;

	float setpoint = inches;
	setpoint *= (countsPerRotation);
	setpoint /= 3.0*PI;


	int distanceL = ((int)setpoint - leftEnc);
	int distanceR = ((int)setpoint - rightEnc);

	tankDrive(distanceExpDec(distanceL, maxSpeed), distanceExpDec(distanceR, maxSpeed));
}
/* Title: driveAngle
 * Parameters: error: angle error value, maxSpeed: max speed allowed for turning
 * Returns: angle positioning loop output
 * Description: processes angle error into a positioning output speed
 */
float angleExpDec(int error, int maxSpeed) {
	float	attenuation = 0.1;
	float answer = 1.0;
	attenuation *= -abs(error);
	answer -= pow(2.718, attenuation);
	answer *= (float) maxSpeed*sgn(error);
	return answer;
}

/* Title: driveAngle
 * Parameters: degrees: specify angle to turn, maxSpeed: top speed allowed for turning
 * Returns: none
 * Description: controls drivetrain heading about the yaw axis
 */
void driveAngle(float degrees, int maxSpeed, int angle) {
	float error = degrees - angle;
	float threshold = 1.0;
	int output = 0;

	if(abs(error) > threshold) {
		output = angleExpDec(error, maxSpeed);
		} else {
		output = 0;
	}
	//writeDebugStreamLine("angle: %f", getAngle());
	tankDrive(output, -output);
}



#else
#error "Motor and sensor configuration file not included!"
#endif
