
void setMotorOut(int power) {
	bool topLimit = false;
	bool bottomLimit = false;

	bool isMovingUp = false;
	bool isMovingDown = false;

	if(topLimit && isMovingUp) {
		power = 0;
		//todo: set encoder max
	}

	if(bottomLimit && isMovingDown) {
		power = 0;
		//todo: zero encoder
	}

	//todo: output to motors
}

void elevatorControlLoop() {

}

task main()
{



}
