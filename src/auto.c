#include "main.h"
#include "robot.h"
#include "ir.h"
#include "opcontrol.h"
#define TASK_STACK_D 70 
#define IR_PRIORITY TASK_PRIORITY_HIGHEST-2
#define MAIN_PRIORITY TASK_PRIORITY_HIGHEST-5

#ifdef ALEN
void ir_task (void * t) {
	printf("Start Alen IR...."); //TODO: atomic?
	while(1){
		slave_loop(); //TODO: user interupts
		taskDelay(10);
	}
}
void alen_auto(void *t){
	while(1){
		printf("Alen main loop\n\r");
		taskDelay(300);
	}
}
#endif
void pascal()//runs around SLAVE
{
#ifdef ALEN
#pragma message ("building auto for Alen")
	printf("Start Alen auto....");
	taskCreate(ir_task,TASK_STACK_D,NULL,IR_PRIORITY);
	taskCreate(alen_auto,TASK_STACK_D,NULL,MAIN_PRIORITY);
	while(1)
		delay(100);
#endif
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
	#ifdef ALEN
	goddard();
	#endif
	#ifdef PASCAL
	pascal();
	#endif
}

