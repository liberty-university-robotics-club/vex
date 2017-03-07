#include "main.h"
#include "robot.h"
#include "ir.h"
#include "opcontrol.h"
#define TASK_STACK_D 70 
#define IR_PRIORITY TASK_PRIORITY_HIGHEST-2
#define MAIN_PRIORITY TASK_PRIORITY_HIGHEST-5

int imecount=0;

#ifdef PASCAL
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
#ifdef PASCAL
#pragma message ("building auto for Alen")
	printf("Start Alen auto....");
	taskCreate(ir_task,TASK_STACK_D,NULL,IR_PRIORITY);
	taskCreate(alen_auto,TASK_STACK_D,NULL,MAIN_PRIORITY);
	while(1)
		delay(100);
#endif
}

int ir_rotate(int power, int ratio_ticks)
{
	static int tick = 0;
	int position;
	imeGet(0, &position);
	if(tick < ratio_ticks)
	{
		//motorSet(MIR,power);
	}
	else if(tick<1000)
	{
		//motorSet(MIR,0);
		power=0;
	}
	else
	{
		tick = 0;
		printf("Reached 1000 ir ticks; position = %d\r\n",position);
		printf("imecount = %d\r\n",imecount);
	}
	tick++;
	
	motorSet(MIR,power);
	return power;
}
void goddard()//base MASTER
{
	#ifdef GODDARD
	
	controldrive(0,0,0);//stopped
	int encoderOffset;
	imeGet(0,&encoderOffset);//starting position
	int enc=0;//centirad
	int power=0;
	int direction = 1;
	while(1)
	{
		//controldrive(0,0,0);
		imeGet(0,&enc);// Experimentally, encoder value of 155 is 90 deg // unit is centirads?
		enc -= encoderOffset;
		
		//Check position to control angular position
		if (enc>=200)//200 = 2 radians
		{
			direction = -1;
			//printf("enc = %d	direction = %d\r\n",enc, direction);
		}
		else if(enc<=0)
		{
			direction = 1;
			//printf("enc = %d	direction = %d\r\n",enc, direction);
		}
		power = ir_rotate(MIR_POW*direction,50);//2nd argument is per 1000
		
		//send byte if stationary or delay if moving
		if (power)
		{
			delay(DELAY_ms);
		}
		else
		{
			//delay(DELAY_ms);
			//turn_on_beacon();
			//send_theta();//goddard on left, so values from 0-115 or so
			//turn off beacon?
		}
		
	}
	
	
	#endif
}
void autonomous()
{
	//system hangs if no ime's to initialize
	printf("imecount = %d\r\n",imecount);
	imecount=imeInitializeAll();
	
	#ifdef GODDARD
	goddard();
	#endif
	#ifdef PASCAL
	pascal();
	#endif
}

