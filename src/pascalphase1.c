
#define MSPEED 64
#include <tank.h>
#include <API.h>
//not opcontrol
//TODO: claw

void pascalphase1(void)
{
		lift_block(1);
		newdriveto(2,1,MSPEED,&ltank);//Y2

}

void pascalphase2(void)
{
	while(1)
	{
		printf("phase2\n\r");
		newdriveto(1.25,1,MSPEED,&ltank);//Y1
		face(&ltank,EAST);
		lift_block(0);
		newdriveto(-2,0,MSPEED,&ltank);//X0
		newdriveto(1,1,MSPEED,&ltank);//Y1
		newdriveto(3,1,MSPEED,&ltank);//Y3 middle of field
		newdriveto(2,0,MSPEED,&ltank);//X3 middle of field
		lift_block(1);
		face(&ltank,NORTH);
		newdriveto(2,0,MSPEED,&ltank);
	}
}
