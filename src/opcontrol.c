#include <math.h>
#include "main.h"
#include "robot.h"
#ifndef API_H_
#include "API.h"
#endif
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

void op_drive()
{
	int joyforwardT  = (abs(joystickGetAnalog(1,JOY_FORWARD_T )) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_FORWARD_T ) : 0;
	int joyforwardS  = (abs(joystickGetAnalog(1,JOY_FORWARD_S )) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_FORWARD_S ) : 0;
	int joyturnT     = (abs(joystickGetAnalog(1,JOY_TURN_T    )) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_TURN_T    ) : 0;
	int joystrafingS = (abs(joystickGetAnalog(1,JOY_STRAFING_S)) > JOY_DEAD_T) ? joystickGetAnalog(1,JOY_STRAFING_S) : 0;
	controldrive(joyturnT,joyforwardT+joyforwardS,joystrafingS);
<<<<<<< HEAD
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
		
	//	// if op buttons pressed, cancel auto button records
	//	if(!lift_pow&&lift_auto){lift_pow=lift_auto;}
	//	else{lift_auto=0;}
		
		#ifdef JOY_LIFT_AUTO
		// if auto buttons pressed, then record state
	//	if( joystickGetDigital( 1 , JOY_LIFT_AUTO , JOY_UP   )
	//	 || joystickGetDigital( 2 , JOY_LIFT_AUTO , JOY_UP   )
	//	){
	//		lift_auto = MLI_POW;
	//	}
	//	if( joystickGetDigital( 1 , JOY_LIFT_AUTO , JOY_DOWN )
	//	 || joystickGetDigital( 2 , JOY_LIFT_AUTO , JOY_DOWN )
	//	){
	//		lift_auto = -MLI_POW;
	//	}
		#endif
	}
	//else //autonomous mode code
	//{
	//	if (begin_auto)
	//	{
	//		lift_auto=go_up?MLI_POW:-MLI_POW;
	//	}
	//}
	//
	//// limit switches - don't stall motors if bottomed out
	//if(lift_pow>0&&  isLiftUp()){lift_pow=0;lift_auto=0;}
	//if(lift_pow<0&&isLiftDown()){lift_pow=0;lift_auto=0;}
	//
	//// Timeout - it shouldn't take that long to raise/lower!
	//if(lift_pow){timer+=.001*DELAY_ms;}
	//else{timer=0;}
	//if(timer > LIFT_TIMEOUT_s){timer=0;lift_pow=0;lift_auto=0;}
	
	// raw motor power
	#ifdef GODDARD
	motorSet(MIR,lift_pow);
	motorSet(MLI,-lift_pow);
	#endif
	#ifdef PASCAL
	motorSet(MIR,-lift_pow);
	motorSet(MLI,lift_pow);
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

/*void UltrasonicActivate(int push_button){
	static int distance1 = 0;
	//static int distance2 = 0;
		//if(US1 == NULL){
			distance1 = ultrasonicGet(US1);
			printf("The distance from the cone is %d centimeters \r\n", distance1);
			delay(1000);
			ultrasonicShutdown(US1);//}
		/*else if(US2 == NULL){
			distance2 = ultrasonicGet(US2);
			printf("The distance from the cone is %d centimeters \r\n", distance2);
			delay(1000);
			ultrasonicShutdown(US2);}*/
		//else if(US1 != NULL /*|| US2 != NULL*/){
			//US1=ultrasonicInit(US_portEcho,US_portPing);
			//US2=ultrasonicInit(10,12);	
	//}
	
//}
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


}


//move to auto.c
void drop_object()//assume lift is at top
{
	//go forward until hit;
	start_auto_lift(false);
	
	
	
	
}

void opcontrol()
{
	/*int push_button=0;
	push_button += joystickGetDigital( 1 , JOY_CLAW , JOY_UP   ) ? 1 : 0 ;
	push_button -= joystickGetDigital( 1 , JOY_CLAW , JOY_DOWN ) ? 1 : 0 ;
	push_button += joystickGetDigital( 0 , JOY_CLAW , JOY_UP   ) ? 1 : 0 ;
	push_button -= joystickGetDigital( 0 , JOY_CLAW , JOY_DOWN ) ? 1 : 0 ;
	printf("%d\r\n",push_button);
	if(push_button == 1){
		UltrasonicActivate(push_button);
		push_button=0;
	}*/

}



void open_claw()
{
	digitalWrite(CLAW_PIN, 1);//open claw
	printf("Open_claw");
}
void close_claw()
{
	digitalWrite(CLAW_PIN, 0);//close claw
	printf("Close_claw");
}

// TODO: maybe reset when done
// maybe not though since done may not occur due to other conditions and
// this would encourage lazy management of the timer.
int waited(int ms)// true if done, false if still waiting.
{
	static int timer = -1;
	if ( timer==-1 || ms==-1 ) // -1 ms means reset timer
	{
		timer=ms;
	}
	else if (timer>0)
	{
		timer-=DELAY_ms;
	}
	//printf("Timer: %d\r\n",timer);
	return !(timer>0);
}
int waited2(int ms)// true if done, false if still waiting.
{
	static int timer = -1;
	if ( timer==-1 || ms==-1 ) // -1 ms means reset timer
	{
		timer=ms;
	}
	else if (timer>0)
	{
		timer-=DELAY_ms;
	}
	//printf("Timer: %d\r\n",timer);
	return !(timer>0);
}

void test_auto_find_cone()
{
	/*
	static int switchflag = 1; // initial direction 1 or -1
	static int state = 0; // 0 is finding cone, 1 is positioning on cone
	static int substate = 0; // depends on state
	static int visibility_state = 0; // 0 is not acquired, 1 is acquired
	static int missed = 0; // time of consecutive ultrasonic drops
	static int width_timer = 0; // keep track of time between edges of cone
	static int last_dist = FAR_DIST+CONE_DELTA; //helps find the closest cone instead of chasing after multiple cones at once
	static int last_button = 0;
	
	int button = 0; // Only if button is pressed
	button += joystickGetDigital( 1 , JOY_AUTO_TEST , JOY_UP   ) ? 1 : 0 ;
	button -= joystickGetDigital( 1 , JOY_AUTO_TEST , JOY_DOWN ) ? 1 : 0 ;
	button += joystickGetDigital( 2 , JOY_AUTO_TEST , JOY_UP   ) ? 1 : 0 ;
	button -= joystickGetDigital( 2 , JOY_AUTO_TEST , JOY_DOWN ) ? 1 : 0 ;
	if(!button) // reset and exit
	{
		state = 0;
		substate = 0;
		visibility_state = 0;
		missed = 0;
		width_timer = 0;
		last_dist = FAR_DIST+CONE_DELTA;
		last_button = 0;
		return;
	}
	
	int dist = ultrasonicGet(US); // current dist to cone
	if (last_button==0)switchflag = button/abs(button);
	
	if (state == 0) // find cone
	{
		if (dist<FAR_DIST+CONE_DELTA && dist>0 
		 && dist < last_dist+CONE_DELTA
		)
		{
			missed = 0;
			if(visibility_state == 0)
			{
				switchflag = -switchflag;
			}
			visibility_state = 1;
			last_dist = dist;
		}
		else
		{
			missed+=DELAY_ms;
			if(missed==5*DELAY_ms)
			{
				visibility_state = 0;
			}
		}
		if(visibility_state == 1)
		{
			if(dist<TARGET_DIST && dist > 0)// changed this
			{
				if (waited(40*DELAY_ms)) // 1 sec
				{
					state = 1;// enter positioning code (strafe)
					printf("Entered state: %d.%d\r\n",state,substate);
				}
				else
				{
					controldrive(0,0,0);
				}
			}
			else
			{
				controldrive(0,TARGET_POW-(FAR_DIST-dist)/3,0);// drive straight
			}
		}
		else
		{
			controldrive(switchflag*TARGET_POW,0,0);// turn
		}
	}
	// maybe add back-up distancing state here?  Might be difficult, esp 
	// if we lose the cone when backing up.  Also, would we want to put 
	// this code after the strafe step?  Or, we could simply make 
	// zeroing in on the cone more repeatable, such as slowing down when 
	// we get close, etc.
	else if (state == 1) // strafe to middle of cone
	{
		if(substate == 0) // 1.0 strafe to right edge
		{
			controldrive(0,0,POS_POW);//strafe
			//controldrive(POS_POW,0,0);//turn
			if (dist<TARGET_DIST && dist>0)
			{
				missed = 0;
				visibility_state = 1;
			}
			else
			{
				missed+=DELAY_ms;
				if(missed==CONE_DELTA*DELAY_ms) //1/2 sec
				{
					substate = 1;
					printf("Entered state: %d.%d\r\n",state,substate);
					visibility_state = 0;
					width_timer = 0;
				}
			}
		}
		else if (substate == 1) // 1.1 strafe to left edge
		{
			controldrive(0,0,-POS_POW);//strafe
			//controldrive(-POS_POW,0,0);//turn
			if ( ( dist<TARGET_DIST && dist>0 ) || width_timer<30)
			{
				missed = 0;
				visibility_state = 1;
			}
			else
			{
				missed+=DELAY_ms;
				if(missed==CONE_DELTA*DELAY_ms)
				{
					substate = 2;
					printf("Entered state: %d.%d\r\n",state,substate);
					visibility_state = 0;
					width_timer /= 2.0;
					waited(WAIT_RESET);
				}
			}
			width_timer++;
		}
		else if (substate == 2) // 1.2 strafe to middle
		{
			controldrive(0,0,POS_POW);//strafe
			//controldrive(POS_POW,0,0);//turn
			if (waited(width_timer*DELAY_ms))
			{
				substate = 3;
				printf("Entered state: %d.%d\r\n",state,substate);
				waited(WAIT_RESET);
			}
		}
		else if (substate == 3) // 1.3 back up proper distance
		{
			if (waited(10*DELAY_ms))
			{
				state = 2;
				substate = 0;
				printf("Entered state: %d.%d\r\n",state,substate);
				waited(WAIT_RESET);
			}
			else
			{
						//controldrive(0,0,0);
				controldrive(0,-POS_POW,0);
						//motorSet(MLIFT,MLIFT_POW);
			}
		}
	}
	else if (state == 2) // 2.0 run claw
	{
		controldrive(0,0,0);
		if (substate == 0) // arm going down
		{
			if (waited(LIFT_TIMER*DELAY_ms))// TODO: tweak
			{
				substate = 1;
				waited(WAIT_RESET);
			}
			else
			{
				motorSet(MLIFT,MLIFT_POW);
			}
		}
		else if (substate == 1) // close claw
		{
			// close claw code here
			motorSet(MLIFT,MLIFT_POW);
			digitalWrite(CLAW_PIN, 1);
			//close_claw();
			substate = 2;
		}
		else if (substate == 2) // Closing claw
		{
			
			if (waited(CLAW_TIMER*DELAY_ms))
			{
				substate = 3;
				motorSet(MLIFT,MLIFT_POW);
				digitalWrite(CLAW_PIN, 1);
				//close_claw();
				waited(WAIT_RESET);
			}
			else
			{
				digitalWrite(CLAW_PIN, 1);
				//close_claw();
			}
		}
		else if (substate == 3) // arm going up
		{
			
			if (waited(LIFT_TIMER*DELAY_ms))
			{
				state = 3;
				substate = 0;
				last_dist = FAR_DIST+CONE_DELTA; // reset find
				//digitalWrite(CLAW_PIN, 0);// 0 open, 1 closed
				waited(WAIT_RESET);
			}
			else
			{
				motorSet(MLIFT,-MLIFT_POW);
			}
		}
	}
	
	
	//------------------------------------------------------------------
	else if (state == 3) // 3.0 find Base
	{
		motorSet(MLIFT,-MLIFT_POW); // arm staying up
		
		if (dist<FAR_DIST+CONE_DELTA && dist>0 
		// && dist < last_dist+CONE_DELTA // <--- find closest object; goal is not
		)
		{
			missed = 0;
			if(visibility_state == 0)
			{
				switchflag = -switchflag;
			}
			visibility_state = 1;
			last_dist = dist;
		}
		else
		{
			missed+=DELAY_ms;
			if(missed==5*DELAY_ms)
			{
				visibility_state = 0;
			}
		}
		if(visibility_state == 1)
		{
			if(dist<TARGET_DIST && dist > 0)// changed this
			{
				if (waited(40*DELAY_ms)) // 1 sec
				{
					state = 4;// enter placing code
					printf("Entered state: %d.%d\r\n",state,substate);
				}
				else
				{
					controldrive(0,0,0);
				}
			}
			else
			{
				controldrive(0,TARGET_POW-(FAR_DIST-dist)/3,0);// drive straight
			}
		}
		else
		{
			controldrive(switchflag*TARGET_POW,0,0);// turn
		}
	}
	
	
	
	last_button=button;
	//*/
}

void wheelie()
{
	static int state;
	static int substate;
	static int last_button;
	int button = 0; // Only if button is pressed
	button += joystickGetDigital( 1 , JOY_AUTO_TEST , JOY_LEFT ) ? 1 : 0 ;
	button -= joystickGetDigital( 1 , JOY_AUTO_TEST , JOY_RIGHT) ? 1 : 0 ;
	button += joystickGetDigital( 2 , JOY_AUTO_TEST , JOY_LEFT ) ? 1 : 0 ;
	button -= joystickGetDigital( 2 , JOY_AUTO_TEST , JOY_RIGHT) ? 1 : 0 ;
	if(!button) // reset and exit
	{
		state = 0;
		substate = 0;
		last_button = 0;
		return;
	}
	if (state == 0)
	{
		waited(WAIT_RESET);//Need more of this.
		waited2(WAIT_RESET);
		state = 1;
		printf("Entered state: %d.%d\r\n",state,substate);
	}
	else if (state == 1)
	{
		if (waited(20*DELAY_ms))//<<<<<-----------This breaks right now.
		{
			state = 2;
			waited(WAIT_RESET);
			printf("Entered state: %d.%d\r\n",state,substate);
		}
		//go backward
		controldrive(0,-POW_WHEELIE,0);
	}
	else if (state == 2)
	{
		if (waited(20*DELAY_ms))
		{
			state = 3;
			waited(WAIT_RESET);
			printf("Entered state: %d.%d\r\n",state,substate);
		}
		//go backward
		controldrive(0,-POW_WHEELIE,0);
		// raise arm
		motorSet(MLIFT,-MLIFT_POW);
	}
	else if (state == 3)
	{
		if (waited(20*DELAY_ms))
		{
			state = 4;
			waited(WAIT_RESET);
			printf("Entered state: %d.%d\r\n",state,substate);
		}
		//go forward
		controldrive(0,POW_WHEELIE,0);
		//lower arm
		motorSet(MLIFT,MLIFT_POW);
	}
	else if (state == 4)
	{
		if (waited(20*DELAY_ms))
		{
			state = 5;
			waited(WAIT_RESET);
			printf("Entered state: %d.%d\r\n",state,substate);
		}
		//stop
	}
	
}

void straigt_forward_auto_score_cone()
{
	static int switchflag = 1; // initial direction 1 or -1
	static int state = 0; // 0 is finding cone, 1 is positioning on cone
	static int substate = 0; // depends on state
	static int visibility_state = 0; // 0 is not acquired, 1 is acquired
	static int missed = 0; // time of consecutive ultrasonic drops
	static int width_timer = 0; // keep track of time between edges of cone
	static int last_dist = FAR_DIST+CONE_DELTA; //helps find the closest cone instead of chasing after multiple cones at once
	static int last_button = 0;
	
	int button = 0; // Only if button is pressed
	button += joystickGetDigital( 1 , JOY_AUTO_TEST , JOY_UP   ) ? 1 : 0 ;
	button -= joystickGetDigital( 1 , JOY_AUTO_TEST , JOY_DOWN ) ? 1 : 0 ;
	button += joystickGetDigital( 2 , JOY_AUTO_TEST , JOY_UP   ) ? 1 : 0 ;
	button -= joystickGetDigital( 2 , JOY_AUTO_TEST , JOY_DOWN ) ? 1 : 0 ;
	if(!button) // reset and exit
	{
		state = 0;
		substate = 0;
		visibility_state = 0;
		missed = 0;
		width_timer = 0;
		last_dist = FAR_DIST+CONE_DELTA;
		last_button = 0;
		return;
	}
	
	int dist = ultrasonicGet(US); // current dist to cone
	if (last_button==0)switchflag = button/abs(button);
	// close claw: digitalWrite(CLAW_PIN, 1); //open is 0
	if (state == 0)
	{
		close_claw();
		state = 1;
	}
	else if (state == 1)
	{
		//digitalWrite(CLAW_PIN, 1);//close claw
		//close_claw();
		motorSet(MLIFT,-MLIFT_POW); // arm staying up
		
		if (waited2(1000) && (waited(4000) || (dist < SORTA_CLOSE && dist > 0) ))
		{
			state = 2;
			waited(WAIT_RESET);
			waited2(WAIT_RESET);
		}
		else
		{
			controldrive(0,TARGET_POW,0);// drive straight
		}
	}
	else if (state == 2) // 3.0 find Base
	{
		//digitalWrite(CLAW_PIN, 1);//close claw
		//close_claw();
		motorSet(MLIFT,-MLIFT_POW); // arm staying up
		
		if (dist<FAR_DIST+CONE_DELTA && dist>0 
		// && dist < last_dist+CONE_DELTA // <--- find closest object; goal is not closest
		)
		{
			missed = 0;
			if(visibility_state == 0)
			{
				switchflag = -switchflag;
			}
			visibility_state = 1;
			last_dist = dist;
		}
		else
		{
			missed+=DELAY_ms;
			if(missed==5*DELAY_ms)
			{
				visibility_state = 0;
			}
		}
		if(visibility_state == 1)
		{
			if(dist<TARGET_DIST && dist > 0)// changed this
			{
				if (waited(40*DELAY_ms)) // 1 sec
				{
					waited(WAIT_RESET);
					state = 3;// enter placing code
					//arm down
					printf("Entered state: %d.%d\r\n",state,substate);
				}
				else
				{
					controldrive(0,0,0);
				}
			}
			else
			{
				controldrive(0,TARGET_POW-(FAR_DIST-dist)/3,0);// drive straight
			}
		}
		else
		{
			controldrive(switchflag*TARGET_POW/2,0,0);// turn
		}
	}
	else if (state == 3)
	{
		if (waited(125))
		{
			waited(WAIT_RESET);
			state = 4;
			controldrive(0,0,0);
		}
		else
		{
			controldrive(0,-128,0);
		}
	}
	else if (state == 4) // 2.0 score
	{
		if (waited(3000))
		{
			waited(WAIT_RESET);
			state = 5;
			open_claw();
		}
		else
		{
			//motorSet(MLIFT,MLIFT_POW); // arm down
			motorSet(MLIFT,0);
		}
	}
	else if (state == 5)
	{
		//open_claw();
	}
	
	last_button=button;
	
}



void stateful_claw_arm()
{
	// ARM ***********************************************************//
	int lift_pow = 0;
	lift_pow += joystickGetDigital( 1 , JOY_LIFT , JOY_UP   ) ? MLIFT_POW : 0 ;
	lift_pow -= joystickGetDigital( 1 , JOY_LIFT , JOY_DOWN ) ? MLIFT_POW : 0 ;
	lift_pow += joystickGetDigital( 2 , JOY_LIFT , JOY_UP   ) ? MLIFT_POW : 0 ;
	lift_pow -= joystickGetDigital( 2 , JOY_LIFT , JOY_DOWN ) ? MLIFT_POW : 0 ;
	
	motorSet(MLIFTLB,-lift_pow);
	motorSet(MLIFTRB, lift_pow);
	motorSet(MLIFTLT,-lift_pow);
	motorSet(MLIFTRT, lift_pow);
	
	
	// CLAW **********************************************************//
	static int last_claw_bool = 0;
	int claw_bool = 0;
	int claw_state = 0;
	claw_state += joystickGetDigital( 1 , JOY_CLAW , JOY_UP   ) ? 1 : 0 ;
	claw_state -= joystickGetDigital( 1 , JOY_CLAW , JOY_DOWN ) ? 1 : 0 ;
	claw_state += joystickGetDigital( 2 , JOY_CLAW , JOY_UP   ) ? 1 : 0 ;
	claw_state -= joystickGetDigital( 2 , JOY_CLAW , JOY_DOWN ) ? 1 : 0 ;
	
	if (claw_state > 0)
	{
		claw_bool = 1;//up - close 
	}
	else if (claw_state < 0)
	{
		claw_bool = 0;//down - open
	}
	else
	{
		claw_bool = last_claw_bool;// don't change
	}
	
	//printf("asdf: %d\r\n",claw_bool);
	if (last_claw_bool != claw_bool || last_claw_bool==10)
	{
		if(claw_bool==10)
		{
			close_claw();//default
		}
		else if(claw_bool==1)
		{
			close_claw();
		}
		else
		{
			open_claw();
		}
	}
	last_claw_bool = claw_bool;
}

//void quick_claw_arm()
//{
//	static int last_claw_bool = 10;
//	int lift_pow = 0;
//	int claw_bool = 0;
//	lift_pow += joystickGetDigital( 1 , JOY_LIFT , JOY_UP   ) ? MLIFT_POW : 0 ;
//	lift_pow -= joystickGetDigital( 1 , JOY_LIFT , JOY_DOWN ) ? MLIFT_POW : 0 ;
//	lift_pow += joystickGetDigital( 2 , JOY_LIFT , JOY_UP   ) ? MLIFT_POW : 0 ;
//	lift_pow -= joystickGetDigital( 2 , JOY_LIFT , JOY_DOWN ) ? MLIFT_POW : 0 ;
//	
//	claw_bool += joystickGetDigital( 1 , JOY_CLAW , JOY_UP   ) ? 1 : 0 ;
//	claw_bool += joystickGetDigital( 2 , JOY_CLAW , JOY_UP   ) ? 1 : 0 ;
//	
//	//printf("asdf: %d\r\n",claw_bool);
//	if (last_claw_bool != claw_bool || last_claw_bool==10)
//	{
//		digitalWrite(CLAW_PIN, claw_bool);
//	}
//	last_claw_bool = claw_bool;
//	motorSet(MLIFT,-lift_pow);
//}







void opcontrol()
{
	op_drive();
	stateful_claw_arm();
	//test_auto_find_cone();
	straigt_forward_auto_score_cone();
	wheelie();
	//op_lift();
	//op_claw();
	//op_hoist();
	//op_hook();//fork lift
}

void inittest()//TODO: remove this lazy
{
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
