#include "main.h"
#include "robot.h"
#include "API.h"
#include <math.h>
#include "liftcontrol.h"
#include "lisp.h"
#include "tank.h"
#include "controlloop.h"

#define DONT_MOVE 0

void controlmotors(int lb, int lf, int rb, int rf)
{
	if(!DONT_MOVE)
	{
		motorSet(MLB,lb);
		motorSet(MLF,lf);
		motorSet(MRB,rb);
		motorSet(MRF,rf);
	}
}
float max (float a, float b)
{
	return a>b?a:b;
}
void controldrive(int t, int f, int s)
{
	static int print = 0;
	print++;
	//printf("titus_controldrive()");
	int strafe = s;
	int turn = t;
	int forward = f;
	
	//Motor power
	//FORWARD_lf is 1 or -1 stating motor direction, forward is joystick magnitude
	//likewise for sideways
	//constants now in robot.h
	int forward_lb = FORWARD_lb * forward;
	int forward_lf = FORWARD_lf * forward;
	int forward_rb = FORWARD_rb * forward;
	int forward_rf = FORWARD_rf * forward;

	int sideways_lb = SIDEWAYS_lb * strafe;
	int sideways_lf = SIDEWAYS_lf * strafe;
	int sideways_rb = SIDEWAYS_rb * strafe;
	int sideways_rf = SIDEWAYS_rf * strafe;
	
	int lb = forward_lb + sideways_lb + turn;
	int lf = forward_lf + sideways_lf + turn;
	int rb = forward_rb + sideways_rb + turn;
	int rf = forward_rf + sideways_rf + turn;
	
	int enc_lb = encoderGet(ENC_LB);
	int enc_lf = encoderGet(ENC_LF);
	int enc_rb = encoderGet(ENC_RB);
	int enc_rf = encoderGet(ENC_RF);
	
	int interval = 100;
	if(!((print+0*interval/4)%interval))
	printf("Power:\t\t%d\t%d\t%d\t%d\n\r",
		lb,
		lf,
		rb,
		rf
	);
	if(!((print+0*interval/4)%interval))
	printf("Raw enc:\t%d\t%d\t%d\t%d\n\r",
		enc_lb,
		enc_lf,
		enc_rb,
		enc_rf
	);
	
	//*
	simtank
	(	&ltank,
		enc_lb,
		enc_lf,
		enc_rb,
		enc_rf
	);//*/
	
	encoderReset(ENC_LB);
	encoderReset(ENC_LF);
	encoderReset(ENC_RB);
	encoderReset(ENC_RF);
	
	lb = controlLoop(enc_lb, lb, NULL);
	lf = controlLoop(enc_lf, lf, NULL);
	rb = controlLoop(enc_rb, rb, NULL);
	rf = controlLoop(enc_rf, rf, NULL);
	
	controlmotors(lb, lf, rb, rf);
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
	manage_lift();
	/*static int once = 1;
	if (once){
		digitalWrite(LIFT_PIN, true);
		once = 0;
	}*/
}


void operatorControl() {

	autonomous(); //TODO: remove this (too lazy to grab joysticks rn)

	//start lisp/UI task
	//taskCreate(lispmain, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);

	while (1) {
		opcontrol();
		delay(20);
	}
}
