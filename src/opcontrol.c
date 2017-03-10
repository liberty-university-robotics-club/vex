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
	(delprint++)%10 ? /*printf("controll!\r\n")*/:delprint;
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

void op_auto_drive(bool auto_mode)// add parameters
{
	int joyforwardT  = (abs(joystickGetAnalog(1,JOY_FORWARD_T )) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_FORWARD_T ) : 0;
	int joyforwardS  = (abs(joystickGetAnalog(1,JOY_FORWARD_S )) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_FORWARD_S ) : 0;
	int joyturnT     = (abs(joystickGetAnalog(1,JOY_TURN_T    )) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_TURN_T    ) : 0;
	int joystrafingS = (abs(joystickGetAnalog(1,JOY_STRAFING_S)) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_STRAFING_S) : 0;
	
	if (!auto_mode)
	{
		
	}
	else
	{
		
	}
	
	controldrive(joyturnT,joyforwardT+joyforwardS,joystrafingS);
}

void op_drive()
{
	op_auto_drive(false);// add arguments
}
//----------------------------------------------------------------------
bool isLiftUp()
{
	return !digitalRead(BTN_UP );//switch pressed is low => 0
}

bool isLiftDown()
{
	return !digitalRead(BTN_DWN);//switch pressed is low => 0
}

int op_auto_lift(bool auto_mode, bool begin_auto, bool go_up)
{
	//first call:
	static int lift_auto=0;
	static int lift_pow=0;
	static float timer=0;
	
	//every call:
	lift_pow=0;
	if(!auto_mode)//if in operator mode:
	{
		// get op button input
		lift_pow += joystickGetDigital( 1 , JOY_LIFT_OP , JOY_UP   ) ? MLI_POW : 0 ;
		lift_pow -= joystickGetDigital( 1 , JOY_LIFT_OP , JOY_DOWN ) ? MLI_POW : 0 ;
		lift_pow += joystickGetDigital( 2 , JOY_LIFT_OP , JOY_UP   ) ? MLI_POW : 0 ;
		lift_pow -= joystickGetDigital( 2 , JOY_LIFT_OP , JOY_DOWN ) ? MLI_POW : 0 ;
		
		lift_pow += joystickGetDigital( 1 , JOY_LIFT_SLOW , JOY_UP   ) ? MLI_SLOW : 0 ;
		lift_pow -= joystickGetDigital( 1 , JOY_LIFT_SLOW , JOY_DOWN ) ? MLI_SLOW : 0 ;
		lift_pow += joystickGetDigital( 2 , JOY_LIFT_SLOW , JOY_UP   ) ? MLI_SLOW : 0 ;
		lift_pow -= joystickGetDigital( 2 , JOY_LIFT_SLOW , JOY_DOWN ) ? MLI_SLOW : 0 ;
		
		// if op buttons pressed, cancel auto button records
		if(!lift_pow&&lift_auto){lift_pow=lift_auto;}
		else{lift_auto=0;}
		
		#ifdef JOY_LIFT_AUTO
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
		#endif
	}
	else //autonomous mode code
	{
		if (begin_auto)
		{
			lift_auto=go_up?MLI_POW:-MLI_POW;
		}
	}
	
	// limit switches - don't stall motors if bottomed out
	if(lift_pow>0&&  isLiftUp()){lift_pow=0;lift_auto=0;}
	if(lift_pow<0&&isLiftDown()){lift_pow=0;lift_auto=0;}
	
	// Timeout - it shouldn't take that long to raise/lower!
	if(lift_pow){timer+=.001*DELAY_ms;}
	else{timer=0;}
	if(timer > LIFT_TIMEOUT_s){timer=0;lift_pow=0;lift_auto=0;}
	
	// raw motor power
	#ifdef GODDARD
	motorSet(MIR,lift_pow);
	motorSet(MLI,-lift_pow);
	#endif
	#ifdef PASCAL
	motorSet(MIR,lift_pow);
	motorSet(MLI,-lift_pow);
	motorSet(MLI2,-lift_pow);
	#endif
	return lift_pow;
}

int auto_lift(bool begin_auto, bool go_up)//generalized version of the two following functions:
{
	return op_auto_lift(true, begin_auto, go_up);
}

void start_auto_lift(bool go_up)// to be called once to start lift
{
	op_auto_lift(true, true, go_up);
}

bool continue_auto_lift()//returns 0 when motor is stopped //to be called to constantly update lift and eventually trigger timeout
{
	return op_auto_lift(true, false, false);
}

void op_lift()
{
	op_auto_lift(false, false, false);
}

//----------------------------------------------------------------------
void op_hoist()
{
	#ifdef GODDARD
	//static float timer=0;
	int hoist_pow = 0;
	// get op button input
	hoist_pow += joystickGetDigital( 1 , JOY_HOIST , JOY_UP   ) ? HOIST_POW : 0 ;
	hoist_pow -= joystickGetDigital( 1 , JOY_HOIST , JOY_DOWN ) ? HOIST_POW : 0 ;
	hoist_pow += joystickGetDigital( 2 , JOY_HOIST , JOY_UP   ) ? HOIST_POW : 0 ;
	hoist_pow -= joystickGetDigital( 2 , JOY_HOIST , JOY_DOWN ) ? HOIST_POW : 0 ;
	
	motorSet(MHOIST,hoist_pow);
	#endif
	//
	////keep track of time
	//if(hoist_pow){timer+=.001*DELAY_ms;}
	//else{timer=0;}
	////set pistons based on time
	////digitalWrite(hoist_pow>0?HOIST1:HOIST2,(!(0.0*HOIST_PERIOD_s < timer && timer < .30*HOIST_PERIOD_s)));
	////digitalWrite(hoist_pow>0?HOIST2:HOIST1,(!(.15*HOIST_PERIOD_s < timer && timer < .75*HOIST_PERIOD_s)));
	////digitalWrite(hoist_pow>0?HOIST1:HOIST2,(!(0.0*HOIST_PERIOD_s < timer && timer < .50*HOIST_PERIOD_s)));
	////digitalWrite(hoist_pow>0?HOIST2:HOIST1,(!(.25*HOIST_PERIOD_s < timer && timer < .75*HOIST_PERIOD_s)));
	//digitalWrite(hoist_pow>0?HOIST1:HOIST2,(!(0.0*HOIST_PERIOD_s < timer && timer < .50*HOIST_PERIOD_s)));
	//digitalWrite(hoist_pow>0?HOIST2:HOIST1,(!(.50*HOIST_PERIOD_s < timer && timer < 1.0*HOIST_PERIOD_s)));
	//if(timer>=HOIST_PERIOD_s)
	//{
	//	//printf("It has been %f secs.\r\n",timer);
	//	timer=0;
	//}
}

void op_hook()
{
	#ifdef GODDARD
	int hook_pow = 0;
	// get op button input
	hook_pow += joystickGetDigital( 1 , JOY_CLAW , JOY_UP   ) ? HOOK_POW : 0 ;
	hook_pow -= joystickGetDigital( 1 , JOY_CLAW , JOY_DOWN ) ? HOOK_POW : 0 ;
	hook_pow += joystickGetDigital( 2 , JOY_CLAW , JOY_UP   ) ? HOOK_POW : 0 ;
	hook_pow -= joystickGetDigital( 2 , JOY_CLAW , JOY_DOWN ) ? HOOK_POW : 0 ;
	
	motorSet(MHOOK,-hook_pow);
	#endif
}
//----------------------------------------------------------------------
int op_auto_claw(bool auto_mode, bool begin_auto, bool clench)
{
	#ifdef PASCAL
	
	/////////////////////////////////////////////
	////simple working op mode code
	////static float timer=0;
	//int claw_pow = 0;
	//
	//if (!auto_mode)
	//{
	//	// get op button input
	//	claw_pow += joystickGetDigital( 1 , JOY_CLAW , JOY_UP   ) ? MCLAW_POW : 0 ;
	//	claw_pow -= joystickGetDigital( 1 , JOY_CLAW , JOY_DOWN ) ? MCLAW_POW : 0 ;
	//	claw_pow += joystickGetDigital( 2 , JOY_CLAW , JOY_UP   ) ? MCLAW_POW : 0 ;
	//	claw_pow -= joystickGetDigital( 2 , JOY_CLAW , JOY_DOWN ) ? MCLAW_POW : 0 ;
	//}
	//else // if (auto_mode)
	//{
	//	//if (begin_auto)
	//	//{
	//	//	claw_auto=clench?MCLAW_POW:-MCLAW_POW;
	//	//}
	//}
	//motorSet(MCLAW,claw_pow);
	/////////////////////////////////////////////
	
	
	//first call:
	static int claw_auto=0;
	static int claw_pow=0;
	static float timer=0;
	
	//every call:
	claw_pow=0;
	if(!auto_mode)//if in operator mode:
	{
		// get op button input
		claw_pow += joystickGetDigital( 1 , JOY_CLAW , JOY_UP   ) ? MCLAW_POW : 0 ;
		claw_pow -= joystickGetDigital( 1 , JOY_CLAW , JOY_DOWN ) ? MCLAW_POW : 0 ;
		claw_pow += joystickGetDigital( 2 , JOY_CLAW , JOY_UP   ) ? MCLAW_POW : 0 ;
		claw_pow -= joystickGetDigital( 2 , JOY_CLAW , JOY_DOWN ) ? MCLAW_POW : 0 ;
		
		// if op buttons pressed, cancel auto button records
		if(!claw_pow&&claw_auto){claw_pow=claw_auto;}
		else{claw_auto=0;}
		
	}
	else //autonomous mode code
	{
		if (begin_auto)
		{
			claw_auto=clench?MCLAW_POW:-MCLAW_POW;
		}
	}
	
	//// Are we sensing this for the claw?
	//// limit switches - don't stall motors if bottomed out
	//if(claw_pow>0&&  isclawUp()){claw_pow=0;claw_auto=0;}
	//if(claw_pow<0&&isclawDown()){claw_pow=0;claw_auto=0;}
	
	// Timeout - it shouldn't take that long to raise/lower!
	if(claw_pow){timer+=.001*DELAY_ms;}
	else{timer=0;}
	if(timer > CLAW_TIMEOUT_s){timer=0;claw_pow=0;claw_auto=0;}
	
	claw_pow*=-1;
	
	// raw motor power
	motorSet(MCLAW,claw_pow);
	return claw_pow;
	
	#endif
	return 0;
}
/////////////////////////////
int auto_claw(bool begin_auto, bool clench)//generalized version of the two following functions:
{
	return op_auto_claw(true, begin_auto, clench);
}

void start_auto_claw(bool clench)// to be called once to start lift
{
	op_auto_claw(true, true, clench);
}

bool continue_auto_claw()//returns 0 when motor is stopped //to be called to constantly update lift and eventually trigger timeout
{
	return op_auto_claw(true, false, false);
}

void op_claw()
{
	op_auto_claw(false, false, false);
}

//move to auto.c
void drop_object()//assume lift is at top
{
	//go forward until hit;
	start_auto_lift(false);
	
	
	
	
}

void opcontrol()
{
	op_drive();
	op_lift();
	op_claw();
	op_hoist();
	op_hook();//fork lift
}

void operatorControl() {

	//autonomous(); //TODO: remove this (too lazy to grab joysticks rn)

	//start lisp/UI task
	//taskCreate(lispmain, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);

	while (1) {
		opcontrol();
		delay(DELAY_ms);
	}
}
