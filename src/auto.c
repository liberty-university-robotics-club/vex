#include "main.h"
#include "robot.h"
#include "ir.h"
#include "opcontrol.h"
#include "tank.h"
#define TASK_STACK_D 70 
#define IR_PRIORITY TASK_PRIORITY_HIGHEST-2
#define MAIN_PRIORITY TASK_PRIORITY_HIGHEST-5

void ir_task (void * t) {
	printf("Start Alen IR...."); //TODO: atomic?
	while(1){
		unsigned int id;
		unsigned int val;
		if(recive_ob(&id,&val)){
			gotflag=1;//allows detecting carrier to get angular position
			printf("Got packet %d %d\n",id,val);
			switch(id){
				case IR_ID_R:constrain(&ltank,val,0);break;
				case IR_ID_T:constrain(&ltank,val,1);break;
				default:printf("Bad packet\n");
			}
			
		}
		taskDelay(200);
	}
}
void lift_block(bool dir)
{
	start_auto_lift(1);
	while(continue_auto_lift()){delay(300);printf("lifting\n\r");}
}
extern void pascalphase1(void);
extern void pascalphase2(void);
void pascal()//runs around SLAVE
{
	taskCreate(ir_task,TASK_STACK_D,NULL,IR_PRIORITY);
	//pascalphase1();
	//pascalphase2();
//	taskCreate(alen_auto,TASK_STACK_D,NULL,MAIN_PRIORITY);
	while(1)
		delay(100);
}
void blastfield(void)
{
	tx_pin=IRT_1;
	transmit_ob(IR_ID_T,IRT_1_T);
		taskDelay(200);
	tx_pin=IRR_1;
	transmit_ob(IR_ID_R,IRR_1_R);
		taskDelay(200);
	tx_pin=IRR_2;
	transmit_ob(IR_ID_R,IRR_2_R);
		taskDelay(200);
	tx_pin=IRR_3;
	transmit_ob(IR_ID_R,IRR_3_R);
		taskDelay(200);
}
void goddard()//base MASTER
{
	controldrive(0,0,0);//stopped
	int encoderOffset;
	imeGet(IME_IR,&encoderOffset);//starting position
	//int enc=0;//deg
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
//	goddard();
	pascal();
}

