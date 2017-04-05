#include "main.h"
#include "robot.h"
#include "ir.h"
#include "opcontrol.h"
#include "tank.h"
#define TASK_STACK_D 70 
#define IR_PRIORITY TASK_PRIORITY_HIGHEST-2
#define MAIN_PRIORITY TASK_PRIORITY_HIGHEST-5

int imecount=0;

#ifdef PASCAL
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
#endif
void lift_block(bool dir)
{
	start_auto_lift(dir);
	while(continue_auto_lift()){delay(300);printf("lifting\n\r");}
}
void claw_block(bool dir)
{
	start_auto_claw(dir);
	while(continue_auto_claw()){delay(300);printf("lifting\n\r");}
}
extern void pascalphase1(void);
extern void pascalphase2(void);
#ifdef PASCAL
void pascal()//runs around SLAVE
{
	printf("Start Alen auto....");
	taskCreate(ir_task,TASK_STACK_D,NULL,IR_PRIORITY);
	//pascalphase1();
	//pascalphase2();
//	taskCreate(alen_auto,TASK_STACK_D,NULL,MAIN_PRIORITY);
	while(1)
		delay(100);
}
#endif
void blastfield(void)
{
	/*tx_pin=IRT_1;
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
	*/
//	tx_pin=IRR_3;
//	transmit_ob(IR_ID_NOP,IRR_3_R);
//		taskDelay(200);
//	tx_pin=IRR_2;
//	transmit_ob(IR_ID_NOP,IRR_3_R);
//		taskDelay(200);
//	tx_pin=IRR_1;
//	transmit_ob(IR_ID_NOP,IRR_3_R);
//		taskDelay(200);
//	tx_pin=IRT_1;
//	transmit_ob(IR_ID_NOP,IRR_3_R);
//		taskDelay(200);
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
#ifdef GODDARD
void goddard()//base MASTER
{
	
	controldrive(0,0,0);//stopped
	int encoderOffset;
	//int enc=0;//deg
	while(1)
	{
		/*controldrive(0,0,0);
		imeGet(IME_IR,&enc);
//>>>>>>> swiley-fail
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
//<<<<<<< HEAD
			//transmit_byte(enc%314);
			transmitob(ID_THETA,enc%314);
		}
//=======
			motorSet(MIR,20);
		}*/
		blastfield();
	}
	
	
}
//<<<<<<< HEAD

#define BLOCKTIME 500
#define TICKTIME 100

void lift_block(bool dir)
{
	start_auto_lift(dir);
	while(continue_auto_lift()){delay(DELAY_ms);printf("lifting\n\r");}
}



void godardsauto(void)
{
	int i;
	lift_block(1);
	for(i=0;i<BLOCKTIME;i=i+TICKTIME)
	{
		controldrive(0,50,0);
		delay(TICKTIME);
	}
}
void autonomous()
{
	//system hangs if no ime's to initialize
	//printf("imecount = %d\r\n",imecount);
	//imecount=imeInitializeAll();
	
	godardsauto();
	
	
	#ifdef GODDARD
	//goddard();
	#endif
	#ifdef PASCAL
	//pascal();
	#endif
//=======
#endif
void autonomous()
{
#ifdef PASCAL
	pascal();
#endif
#ifdef GODDARD
	goddard();
#endif
//>>>>>>> 37e7ebb0b98417cd39299a83a23ccb31f6a7f179
}

