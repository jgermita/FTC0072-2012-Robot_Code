float gyro_bias = 0.0;	//Gyro bias value, calculated at init
float angle = 0.0;			//Calculated angle value;
float angle2 = 0.0;			//Output angle value for safe multithreading
int time = 0;						//Time value for integration
int prevTime = 0;				//previous time value for integration
float maxRate = 270.0;	//Maximum allowable degrees/second rate. input gyro rate is clipped to this
float gyro_old = 0 ;		//old gyro value for integration

/* Title: initGyro
 * Parameters: none
 * Returns: none
 * Description: Initializes gyro by calculating gyro bias and resetting timers
 */
void initGyro() {
	int biasDelay = 20;
	for(int i = 0; i < 20; i++)  {		//Iterate through the loop 20 times to calculate the gyro bias
		gyro_bias += SensorValue[Gyro];
		wait1Msec(biasDelay);
	}
	gyro_bias /= 20;	//Average
	writeDebugStreamLine("GyroBias is : %d", gyro_bias);
	prevTime = nPgmTime;
	writeDebugStreamLine("Gyro is initialized!");
}


/* Title: updateHeading
 * Parameters: none
 * Returns: none
 * Description: calculates yaw angle by using trapezoidal integration on a filtered dtheta input
 */
void updateHeading() {
	float driftThresh = .9;	//Minimum allowable dtheta/dt to integrate
	float delay = 10;				//Trapezoidal integration delay between interations
	prevTime = time;				//setting time variables for dt
	time = nPgmTime;

	int dt = abs(time - prevTime);	//Calculate delta time
	if(dt == 0) dt = 1;							//Prevent div/0

	float gyro_new = SensorValue[Gyro] - (float)gyro_bias;		//Input sensor value with gyro bias compensation
	if(abs(gyro_new) < driftThresh) gyro_new = 0.0;						//Filter input, if lower than a threshold, zero the new value
	if(abs(gyro_new) >= maxRate) gyro_new = maxRate*sgn(gyro_new);	//filter input, if higher than a threshold, cap at that threshold

	angle += (float)dt*0.5*(gyro_new+gyro_old)/1000.0;	//trapezoidal integration of the gyro rate


	//if(angle >= 180.0) {
	//	angle -= 360.0;
	//} else if(angle <= -180.0) {
	//	angle += 360.0;
	//}

	gyro_old = gyro_new;
	wait1Msec(delay);
}

/* Title: getAngle()
 * Parameters: none
 * Returns: gyro angle in degrees
 * Description: returns a gyro angle
 */
float getAngle() {
	return angle2;
}

task gyroTask() {
	initGyro();
	while(true) {
		updateHeading();
		angle2 = angle;
	}
}
