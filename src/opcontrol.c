#include <math.h>
#include "main.h"
#include "robot.h"
#include "API.h"
#include "liftcontrol.h"
#include "lisp.h"
#include "tank.h"
#include "controlloop.h"
#include "opcontrol.h"

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
//float max (float a, float b)
//{
//	return a>b?a:b;
//}
double cfmin(double a, double b, double c, double d)
{
	double min = fmin(fmin(fabs(a),fabs(b)),fmin(fabs(c),fabs(d)));
	return min?17:min;
}
void controldrive(int t, int f, int s)
{
	static int print = 0;
	print++;
	/*
	static state control_lb = { .speed = 0 };
	static state control_lf = { .speed = 0 };
	static state control_rb = { .speed = 0 };
	static state control_rf = { .speed = 0 };
	//*/
	
	static double last_pwr_lb = 100;
	static double last_pwr_lf = 100;
	static double last_pwr_rb = 100;
	static double last_pwr_rf = 100;
	
	
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
	
	int new_lb = lb;
	int new_lf = lf;
	int new_rb = rb;
	int new_rf = rf;
	
	int enc_lb = encoderGet(ENC_LB);
	int enc_lf = encoderGet(ENC_LF);
	int enc_rb = encoderGet(ENC_RB);
	int enc_rf = encoderGet(ENC_RF);
	
	int interval = 100;
	if(!((print+0*interval/4)%interval))
	printf("\n\rPower:\t\t%d\t%d\t%d\t%d\n\r",
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
	
	double min = cfmin(
		1.0*enc_lb/last_pwr_lb,
		1.0*enc_lf/last_pwr_lf,
		1.0*enc_rb/last_pwr_rb,
		1.0*enc_rf/last_pwr_rf
	);
	
	new_lb *= fabs(min*last_pwr_lb/enc_lb?MAGIC_NUMBER:enc_lb);
	new_lf *= fabs(min*last_pwr_lf/enc_lf?MAGIC_NUMBER:enc_lf);
	new_rb *= fabs(min*last_pwr_rb/enc_rb?MAGIC_NUMBER:enc_rb);
	new_rf *= fabs(min*last_pwr_rf/enc_rf?MAGIC_NUMBER:enc_rf);
	
	last_pwr_lb = lb;
	last_pwr_lf = lf;
	last_pwr_rb = rb;
	last_pwr_rf = rf;
	
	if(!((print+0*interval/4)%interval))
	printf("Scaled Pwr:\t%d\t%d\t%d\t%d\n\r",
		new_lb,
		new_lf,
		new_rb,
		new_rf
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
	
	controlmotors(
		elijah_controlLoop(enc_lb, lb, last_pwr_lb),//new_lb,
		elijah_controlLoop(enc_lf, lf, last_pwr_lf),//new_lf,
		elijah_controlLoop(enc_rb, rb, last_pwr_rb),//new_rb,
		elijah_controlLoop(enc_rf, rf, last_pwr_rf) //new_rf
	);
	//*
	//lb = controlLoop(enc_lb, lb, &control_lb);
	//lf = controlLoop(enc_lf, lf, &control_lf);
	//rb = controlLoop(enc_rb, rb, &control_rb);
	//rf = controlLoop(enc_rf, rf, &control_rf);
	
	lb = elijah_controlLoop(enc_lb, lb, last_pwr_lb);
	lf = elijah_controlLoop(enc_lf, lf, last_pwr_lf);
	rb = elijah_controlLoop(enc_rb, rb, last_pwr_rb);
	rf = elijah_controlLoop(enc_rf, rf, last_pwr_rf);
	
	if(!((print+0*interval/4)%interval))
	printf("Control:\t%d\t%d\t%d\t%d\n\r",
		lb,
		lf,
		rb,
		rf
	);
	//*/
}

void driveoperation()
{
	int joyforwardT  = (abs(joystickGetAnalog(1,JOY_FORWARD_T )) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_FORWARD_T ) : 0;
	int joyforwardS  = (abs(joystickGetAnalog(1,JOY_FORWARD_S )) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_FORWARD_S ) : 0;
	int joyturnT     = (abs(joystickGetAnalog(1,JOY_TURN_T    )) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_TURN_T    ) : 0;
	int joystrafingS = (abs(joystickGetAnalog(1,JOY_STRAFING_S)) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_STRAFING_S) : 0;

	controldrive(joyturnT,joyforwardT+joyforwardS,joystrafingS);
}
bool isLiftUp()
{
	return !digitalRead(BTN_UP );//switch pressed is low => 0
}
bool isLiftDown()
{
	return !digitalRead(BTN_DWN);
}
void op_auto_lift()
{
	//first call:
	static int lift_auto=0;
	static int lift_pow=0;
	
	//every call:
	lift_pow=0;
	lift_pow += joystickGetDigital( 1 , JOY_LIFT_OP , JOY_UP   ) ? MLI_POW : 0 ;
	lift_pow -= joystickGetDigital( 1 , JOY_LIFT_OP , JOY_DOWN ) ? MLI_POW : 0 ;
	lift_pow += joystickGetDigital( 2 , JOY_LIFT_OP , JOY_UP   ) ? MLI_POW : 0 ;
	lift_pow -= joystickGetDigital( 2 , JOY_LIFT_OP , JOY_DOWN ) ? MLI_POW : 0 ;
	
	if(!lift_pow&&lift_auto){lift_pow=lift_auto;}
	else{lift_auto=0;}
	
	if( joystickGetDigital( 1 , JOY_LIFT_AUTO , JOY_UP   )
	 || joystickGetDigital( 2 , JOY_LIFT_AUTO , JOY_UP   )
	){
		lift_auto = MLI_POW;
	}
	if( joystickGetDigital( 1 , JOY_LIFT_AUTO , JOY_DOWN )
	 || joystickGetDigital( 2 , JOY_LIFT_AUTO , JOY_DOWN )
	){
		lift_auto = -MLI_POW;
	}
	
	if(lift_pow>0&&  isLiftUp()){lift_pow=0;lift_auto=0;}
	if(lift_pow<0&&isLiftDown()){lift_pow=0;lift_auto=0;}
	printf("motorSet(MLI,%d)\r\n",lift_pow);
	motorSet(MLI,lift_pow);
}

void opcontrol()
{
	//driveoperation();
	op_auto_lift();
	//manage_lift();
	/*static int once = 1;
	if (once){
		digitalWrite(LIFT_PIN, true);
		once = 0;
	}*/
}

void operatorControl() {

	//autonomous(); //TODO: remove this (too lazy to grab joysticks rn)

	//start lisp/UI task
	//taskCreate(lispmain, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);

	while (1) {
		opcontrol();
		delay(250);
	}
}
