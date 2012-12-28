#define IDLE 0
#define ACTIVE 1
#define CRITICAL 2
#define SUPER_CRITICAL 3

#include "drivers/hitechnic-protoboard.h"

int currMode = 0;

void setMode(int mode) {
	hogCPU();
	currMode = mode;
	releaseCPU();
}

task SignalLight()
{
	HTPBsetupIO(HTPB, 0x3F);
	while(true) {
		int onTime = 0;
		int offTime = 0;

		if(currMode == IDLE) {
			onTime = 1875;
			offTime = 125;
		} else if(currMode == ACTIVE) {
			onTime = 950;
			offTime = 50;
		} else if(currMode == CRITICAL) {
			onTime = 125;
			offTime = 125;
		} else if(currMode == SUPER_CRITICAL) {
			onTime = 63;
			offTime = 63;
		}

		HTPBwriteIO(HTPB, 0x3f);
		wait1Msec(onTime);
		HTPBwriteIO(HTPB, 0x00);
		wait1Msec(offTime);
	}


}
