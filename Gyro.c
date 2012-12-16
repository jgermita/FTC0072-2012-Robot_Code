float gyro_bias = 0.0;
float angle = 0.0;
float angle2 = 0.0;
int time = 0;
int prevTime = 0;
float maxRate = 180.0;
float gyro_old = 0 ;

void initGyro() {
	int biasDelay = 20;
	for(int i = 0; i < 20; i++)  {
		gyro_bias += SensorValue[Gyro];
		wait1Msec(biasDelay);
	}
	gyro_bias /= 20;
	writeDebugStreamLine("GyroBias is : %d", gyro_bias);
	prevTime = nPgmTime;
}

void updateHeading() {
	float driftThresh = .9;
	float delay = 10;
	prevTime = time;
	time = nPgmTime;

	int dt = abs(time - prevTime);
	if(dt == 0) dt = 1;

	float gyro_new = SensorValue[Gyro] - (float)gyro_bias;
	if(abs(gyro_new) < driftThresh) gyro_new = 0.0;
	if(abs(gyro_new) >= maxRate) gyro_new = maxRate*sgn(gyro_new);

	angle += (float)dt*0.5*(gyro_new+gyro_old)/1000.0;


	if(angle >= 360.0) {
		angle -= 360.0;
	} else if(angle <= 0) {
		angle += 360.0;
	}

	gyro_old = gyro_new;
	wait1Msec(delay);
}
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
