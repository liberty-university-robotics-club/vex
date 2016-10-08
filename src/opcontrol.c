#include "main.h"
#include "robot.h"
#include "API.h"
#include <math.h>
#include "liftcontrol.h"

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
	
	int lb;
	int lf;
	int rb;
	int rf;
	
	//*
	static float last_lb = 1;
	static float last_lf = 1;
	static float last_rb = 1;
	static float last_rf = 1;
	//*/
	
	/*
	static float suggested_lb = 1.0;
	static float suggested_lf = 1.0;
	static float suggested_rb = 1.0;
	static float suggested_rf = 1.0;
	//*/
	
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
	
	lb = forward_lb + sideways_lb;
	lf = forward_lf + sideways_lf;
	rb = forward_rb + sideways_rb;
	rf = forward_rf + sideways_rf;

	lb+=turn;
	lf+=turn;
	rb+=turn;
	rf+=turn;
	
	//Sensor data
	float scaled_speed_lb = encoderGet(ENC_LB) * (SCALED_SPEED_CONVERSION) * (FORWARD_lb);
	float scaled_speed_lf = encoderGet(ENC_LF) * (SCALED_SPEED_CONVERSION) * (FORWARD_lf);
	float scaled_speed_rb = encoderGet(ENC_RB) * (SCALED_SPEED_CONVERSION) * (FORWARD_rb);
	float scaled_speed_rf = encoderGet(ENC_RF) * (SCALED_SPEED_CONVERSION) * (FORWARD_rf);
	
	float weighted_lb = 1.0*last_lb/scaled_speed_lb;
	float weighted_lf = 1.0*last_lf/scaled_speed_lf;
	float weighted_rb = 1.0*last_rb/scaled_speed_rb;
	float weighted_rf = 1.0*last_rf/scaled_speed_rf;
	
	float weighted_max = max(
		max(
			weighted_lb,
			weighted_lf),max(
			weighted_rb,
			weighted_rf
		)
	);
	
	weighted_lb /= weighted_max;
	weighted_lf /= weighted_max;
	weighted_rb /= weighted_max;
	weighted_rf /= weighted_max;
	
	int interval = 100;
	if(!((print+0*interval/4)%interval))
	printf("Power:\t\t%d\t%d\t%d\t%d\r\n",
		lb,
		lf,
		rb,
		rf
	);
	if(!((print+0*interval/4)%interval))
	printf("Raw enc:\t%d\t%d\t%d\t%d\r\n",
		encoderGet(ENC_LB),
		encoderGet(ENC_LF),
		encoderGet(ENC_RB),
		encoderGet(ENC_RF)
	);
	if(!((print+0*interval/4)%interval))
	printf("Scaled:\t%f\t%f\t%f\t%f\r\n",
		scaled_speed_lb,
		scaled_speed_lf,
		scaled_speed_rb,
		scaled_speed_rf
	);
	if(!((print+0*interval/4)%interval))
	printf("Scld/Pwr:\t%f\t%f\t%f\t%f\r\n",
		weighted_lb,
		weighted_lf,
		weighted_rb,
		weighted_rf
	);
	
	/*
	simtank
	(	&ltank,
		encoderGet(ENC_LB),
		encoderGet(ENC_LF),
		encoderGet(ENC_RB),
		encoderGet(ENC_RF)
	);//*/
	
	encoderReset(ENC_LB);
	encoderReset(ENC_LF);
	encoderReset(ENC_RB);
	encoderReset(ENC_RF);
	
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
	taskCreate(lispmain, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);

	while (1) {
		opcontrol();
		delay(20);
	}
}
