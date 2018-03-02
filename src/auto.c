#include "main.h"
#include "robot.h"
#include "ir.h"
#include "opcontrol.h"
#include "tank.h"
#define TASK_STACK_D 70 
#define IR_PRIORITY TASK_PRIORITY_HIGHEST-2
#define MAIN_PRIORITY TASK_PRIORITY_HIGHEST-5

int imecount=0;

void autonomous()
{
	//system hangs if no ime's to initialize
	//printf("imecount = %d\r\n",imecount);
	//imecount=imeInitializeAll();
	
	int switchflag = SWITCHFLAG; // initial direction 1 or -1
	int state = 0; // 0 is finding cone, 1 is positioning on cone
	int substate = 0; // depends on state
	int visibility_state = 0; // 0 is not acquired, 1 is acquired
	int missed = 0; // time of consecutive ultrasonic drops
	int width_timer = 0; // keep track of time between edges of cone
	int last_dist = FAR_DIST+CONE_DELTA; //helps find the closest cone instead of chasing after multiple cones at once
	int dist;
	
	while(1)
	{
		#ifdef WALTER
		dist = ultrasonicGet(US); // current dist to cone
		
		
		if (state == 0)
		{
			//close_claw();
			//printf("%d",waited3(500));
			if (waited3(500))
			{
				printf("motorSet(MHOOK,0);");
				motorSet(MHOOK,0);
				state = 1;
				waited3(WAIT_RESET);
			}
			else
			{
				printf("motorSet(MHOOK,CLAW_POW);");
				motorSet(MHOOK,CLAW_POW);// +CCW
			}
		}
		else if (state == 1)
		{
			//digitalWrite(CLAW_PIN, 1);//close claw
			//close_claw();
			
			lift_arm(MLIFT_POW); // arm staying up
			
			if (waited2(1000) && (waited(4000) || (dist < SORTA_CLOSE && dist > 0) ))
			{
				printf("RESET");
				state = 2;
				waited(WAIT_RESET);
				waited2(WAIT_RESET);
			}
			else
			{
				controldrive(0,GOAL_POW,0);// drive straight
			}
		}
		else if (state == 2) // 3.0 find Base
		{
			//digitalWrite(CLAW_PIN, 1);//close claw
			//close_claw();
			lift_arm(MLIFT_POW); // arm staying up
			
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
					controldrive(0,GOAL_POW-(FAR_DIST-dist)/3,0);// drive straight
				}
			}
			else
			{
				controldrive(switchflag*GOAL_POW/2,0,0);// turn
			}
		}
		else if (state == 3)// back up for positioning
		{
			//if (waited(125))
			//{
			//	waited(WAIT_RESET);
				state = 4;
				controldrive(0,0,0);
			//}
			//else
			//{
			//	controldrive(0,-128,0);
			//}
		}
		else if (state == 4) // 2.0 score
		{
			if (waited(3000))
			{
				printf("Open Claw");
				waited(WAIT_RESET);
				state = 5;
				open_claw();
				lift_arm(0);
			}
			else
			{
				//motorSet(MLIFT,MLIFT_POW); // arm down
				lift_arm(-MLIFT_SLOW);
			}
		}
		else if (state == 5)//open claw
		{
			if (waited(1000))
			{
				state = 6;
				stop_claw();
				waited(WAIT_RESET);
			}
			else
			{
				open_claw();
			}
		}
		else if (state == 6)// raise fork
		{
			if (waited(3500))
			{
				state = 7;
				waited(WAIT_RESET);
				raise_fork(0);
			}
			else
			{
				raise_fork(HOIST_POW);
			}
		}
		else if (state == 7)//back up
		{
			if (waited(3000))
			{
				state = 8;
				waited(WAIT_RESET);
				controldrive(0,0,0);
			}
			else
			{
				//strafe
				controldrive(0,-switchflag*GOAL_POW,0);
			}
		}
		
		
		#endif
		//--------------------------------------------------------------
		//==============================================================
		//--------------------------------------------------------------
		// Begin safety net autonomous code for IMP:
		#ifdef FREAKING_OUT
		//dist = ultrasonicGet(US); // current dist to cone
		//if (state == 0)
		//{
		//	close_claw();
		//	state = 1;
		//}
		//else if (state == 1)
		//{
		//	lift_arm(MLIFT_POW); // arm staying up
		//	
		//	if (waited2(1000) && (waited(4000) || (dist < SORTA_CLOSE && dist > 0) ))
		//	{
		//		state = 2;
		//		waited(WAIT_RESET);
		//		waited2(WAIT_RESET);
		//	}
		//	else
		//	{
		//		controldrive(0,TARGET_POW,0);// drive straight
		//	}
		//}
		//else if (state == 2) // 3.0 find Base
		//{
		//	lift_arm(MLIFT_POW); // arm staying up
		//	
		//	if (dist<FAR_DIST+CONE_DELTA && dist>0 
		//	)
		//	{
		//		missed = 0;
		//		if(visibility_state == 0)
		//		{
		//			switchflag = -switchflag;
		//		}
		//		visibility_state = 1;
		//		last_dist = dist;
		//	}
		//	else
		//	{
		//		missed+=DELAY_ms;
		//		if(missed==5*DELAY_ms)
		//		{
		//			visibility_state = 0;
		//		}
		//	}
		//	if(visibility_state == 1)
		//	{
		//		if(dist<TARGET_DIST && dist > 0)// changed this
		//		{
		//			if (waited(40*DELAY_ms)) // 1 sec
		//			{
		//				waited(WAIT_RESET);
		//				state = 3;// enter placing code
		//				//arm down
		//				printf("Entered state: %d.%d\r\n",state,substate);
		//			}
		//			else
		//			{
		//				controldrive(0,0,0);
		//			}
		//		}
		//		else
		//		{
		//			controldrive(0,TARGET_POW-(FAR_DIST-dist)/3,0);// drive straight
		//		}
		//	}
		//	else
		//	{
		//		controldrive(switchflag*TARGET_POW/2,0,0);// turn
		//	}
		//}
		//else if (state == 3) // 2.0 score
		//{
		//	if (waited(500))
		//	{
		//		waited(WAIT_RESET);
		//		state = 4;
		//		open_claw();
		//	}
		//	else
		//	{
		//		lift_arm(-MLIFT_SLOW); // arm down
		//	}
		//}
		//else if (state == 4)
		//{
		//	//done
		//}
		#endif
		//delay ms
		delay(DELAY_ms);	
	}
}

