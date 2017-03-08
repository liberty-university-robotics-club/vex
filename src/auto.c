#include "main.h"
#include "robot.h"
#include "ir.h"
#include "opcontrol.h"
#define TASK_STACK_D 70 
#define IR_PRIORITY TASK_PRIORITY_HIGHEST-2
#define MAIN_PRIORITY TASK_PRIORITY_HIGHEST-5

void ir_task (void * t) {
	printf("Start Alen IR...."); //TODO: atomic?
	while(1){
		slave_loop(); //TODO: user interupts
		taskDelay(10);
	}
}
void lift_block(bool dir)
{
	start_auto_lift(dir);
	while(continue_auto_lift())delay(300);
}
extern void pascalphase1(void);
extern void pascalphase2(void);
void pascal()//runs around SLAVE
{
	taskCreate(ir_task,TASK_STACK_D,NULL,IR_PRIORITY);
	pascalphase1();
//	taskCreate(alen_auto,TASK_STACK_D,NULL,MAIN_PRIORITY);
//	while(1)
//		delay(100);
}
void goddard()//base MASTER
{
	controldrive(0,0,0);//stopped
	int encoderOffset;
	imeGet(IME_IR,&encoderOffset);//starting position
	int enc=0;//deg
	while(1)
	{
		controldrive(0,0,0);
		imeGet(IME_IR,&enc);
		enc -= encoderOffset;
		if(!DONT_MOVE)
		{
			motorSet(MIR,20);
		}
	}
	//turn_on_beacon();
	//send_theta();//goddard on left, so values from 0-115 or so
	
	
}
void autonomous()
{
	goddard();
	pascal();
}

