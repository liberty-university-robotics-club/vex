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
		int id;
		int val;
		if(recive_ob(&id,&val)){
			gotflag=1;//allows detecting carrier to get angular position
			printf("Got packet %d %d\n");
			switch(id){
				case IR_ID_R:constrain(&ltank,val,0);break;
				case IR_ID_T:constrain(&ltank,val,1);break;
				default:printf("Bad packet\n");
			}
			
		}
		task_delay(200);
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
void blastfield(void)
{
	ir_pin=IRT_1;
	transmitob(IRID_T,IRT_1_T);
	ir_pin=IRR_1;
	transmitob(IRID_T,IRR_1_R);
	ir_pin=IRR_2;
	transmitob(IRID_T,IRR_2_R);
	ir_pin=IRR_3;
	transmitob(IRID_T,IRR_3_R);
}
void goddard()//base MASTER
{
	controldrive(0,0,0);//stopped
	int encoderOffset;
	imeGet(IME_IR,&encoderOffset);//starting position
	int enc=0;//deg
	while(1)
	{
		/*controldrive(0,0,0);
		imeGet(IME_IR,&enc);
		enc -= encoderOffset;
		if(!DONT_MOVE)
		{
			motorSet(MIR,20);
		}*/
		blastfield();
	}
	//turn_on_beacon();
	//send_theta();//goddard on left, so values from 0-115 or so
	
	
}
void autonomous()
{
	goddard();
	pascal();
}

