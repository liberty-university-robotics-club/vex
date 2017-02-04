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
		motorSet(MLB,lb/2);
		motorSet(MLF,lf/2);
		motorSet(MRB,rb/2);
		motorSet(MRF,rf/2);
	}
}

void controldrive(int t, int f, int s)
{
	static int delprint=0;
	(delprint++)%10 ? printf("controll!\n"):delprint;
	static state *state_lb=NULL;
	static state *state_lf=NULL;
	static state *state_rb=NULL;
	static state *state_rf=NULL;

	int f_lb = FORWARD_lb * f;
	int f_lf = FORWARD_lf * f;
	int f_rb = FORWARD_rb * f;
	int f_rf = FORWARD_rf * f;

	int sideways_lb = SIDEWAYS_lb * s;
	int sideways_lf = SIDEWAYS_lf * s;
	int sideways_rb = SIDEWAYS_rb * s;
	int sideways_rf = SIDEWAYS_rf * s;
	
	int lb = f_lb + sideways_lb + t;
	int lf = f_lf + sideways_lf + t;
	int rb = f_rb + sideways_rb + t;
	int rf = f_rf + sideways_rf + t;

	//deadreconning
	int enc_lb = encoderGet(ENC_LB);
	int enc_lf = encoderGet(ENC_LF);
	int enc_rb = encoderGet(ENC_RB);
	int enc_rf = encoderGet(ENC_RF);
	simtank
	(	&ltank,
		enc_lb,
		enc_lf,
		enc_rb,
		enc_rf
	);
	
	lb=controlLoop(enc_lb,lb,state_lb);
	lf=controlLoop(enc_lf,lf,state_lf);
	rb=controlLoop(enc_rb,rb,state_rb);
	rf=controlLoop(enc_rf,rf,state_rf);

	
	encoderReset(ENC_LB);
	encoderReset(ENC_LF);
	encoderReset(ENC_RB);
	encoderReset(ENC_RF);
	
	controlmotors(
		lb,
		lf,
		rb,
		rf
	);
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
	return !digitalRead(BTN_DWN);//switch pressed is low => 0
}

void op_auto_lift()
{
	//first call:
	static int lift_auto=0;
	static int lift_pow=0;
	
	//every call:
	// get op button input
	lift_pow=0;
	lift_pow += joystickGetDigital( 1 , JOY_LIFT_OP , JOY_UP   ) ? MLI_POW : 0 ;
	lift_pow -= joystickGetDigital( 1 , JOY_LIFT_OP , JOY_DOWN ) ? MLI_POW : 0 ;
	lift_pow += joystickGetDigital( 2 , JOY_LIFT_OP , JOY_UP   ) ? MLI_POW : 0 ;
	lift_pow -= joystickGetDigital( 2 , JOY_LIFT_OP , JOY_DOWN ) ? MLI_POW : 0 ;
	
	// if op buttons pressed, cancel auto button records
	if(!lift_pow&&lift_auto){lift_pow=lift_auto;}
	else{lift_auto=0;}
	
	// if auto buttons pressed, then record state
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
	
	// limit switches - don't stall motors if bottomed out
	if(lift_pow>0&&  isLiftUp()){lift_pow=0;lift_auto=0;}
	if(lift_pow<0&&isLiftDown()){lift_pow=0;lift_auto=0;}
	
	//TODO: Add timer
	
	// raw motor power
	motorSet(MLI,lift_pow);
}

void opcontrol()
{
	driveoperation();
	op_auto_lift();
}

void operatorControl() {

	//autonomous(); //TODO: remove this (too lazy to grab joysticks rn)

	//start lisp/UI task
	//taskCreate(lispmain, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);

	while (1) {
		opcontrol();
		delay(25);
	}
}
