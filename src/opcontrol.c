#include "main.h"
#include "robot.h"
#include "API.h"
#include <math.h>


#define DONT_MOVE 0

void controlmotors(int lb, int rb, int lf, int rf)
{
	if(!DONT_MOVE)
	{
		motorSet(MLB,lb);
		motorSet(MLF,lf);
		motorSet(MRB,rb);
		motorSet(MRF,rf);
	}
}

void controldrive(int t, int f, int s)
{
	printf("titus_controldrive()");
	int strafe = s;
	int turn = t;
	int forward = f;
	int lf,lb,rf,rb;
	
	static int last_lf = 0;
	static int last_lb = 0;
	static int last_rf = 0;
	static int last_rb = 0;
	
	
	//Motor power
	//FORWARD_lf is 1 or -1 stating motor direction, forward is joystick magnitude
	//likewise for sideways
	int forward_lf = FORWARD_lf * forward;
	int forward_lb = FORWARD_lb * forward;
	int forward_rf = FORWARD_rf * forward;
	int forward_rb = FORWARD_rb * forward;

	int sideways_lf = SIDEWAYS_lf * strafe;
	int sideways_lb = SIDEWAYS_lb * strafe;
	int sideways_rf = SIDEWAYS_rf * strafe;
	int sideways_rb = SIDEWAYS_rb * strafe;
	
	
	
	lf = forward_lf + sideways_lf;
	lb = forward_lb + sideways_lb;
	rf = forward_rf + sideways_rf;
	rb = forward_rb + sideways_rb;

	lf+=turn;
	lb+=turn;
	rf+=turn;
	rb+=turn;
	
	//Sensor data
	/*
	simtank
	(
		&ltank,
		
		encoderGet(ENC_LB),
		encoderGet(ENC_LF),
		encoderGet(ENC_RB),
		encoderGet(ENC_RF)
	);//*/
	
	encoderReset(ENC_LB);
	encoderReset(ENC_LF);
	encoderReset(ENC_RB);
	encoderReset(ENC_RF);
	
	controlmotors(lb, rb, lf, rf);
}

void driveoperation()
{
	int joyforwardT  = (abs(joystickGetAnalog(1,JOY_FORWARD_T )) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_FORWARD_T ) : 0;
	int joyforwardS  = (abs(joystickGetAnalog(1,JOY_FORWARD_S )) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_FORWARD_S ) : 0;
	int joyturnT     = (abs(joystickGetAnalog(1,JOY_TURN_T    )) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_TURN_T    ) : 0;
	int joystrafingS = (abs(joystickGetAnalog(1,JOY_STRAFING_S)) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_STRAFING_S) : 0;

	controldrive(joyturnT,joyforwardT+joyforwardS,joystrafingS);
}

void opcontrol()
{
	driveoperation();
}

void operatorControl() {

	autonomous(); //TODO: remove this (too lazy to grab joysticks rn)
	while (1) {
		opcontrol();
		delay(20);
	}
}
