#include <stdio.h>
#include <stdlib.h>
#include <sim.h>
#include <math.h>
#include "robot.h"
#define SIMTIME 1000
int axis;
int value;
int bad=0;
int good=0;
int iterations;
//static tests
int axis,value=0;
int sign (int a) {
	if(a==0)
		return 0; //+1!=0!=-1
	return abs(a)/a;
}
void die(char *a) {
	printf("\n %s\nbad:%d\ngood:%d / %d\n",a,bad,good,iterations);
	if(bad==0 && good==iterations)
		printf("All PASSED!\n");
	else
		printf("some failures\n");
	exit(0);
}
void sim_nextjoy() {
	axis=(sim_clock/20)%2;
	value=(((sim_clock/40)%3)*200)-200;
	int i=0;
	for(i=0;i<11;i++)
		joy[i][AXIS]=0;
	joy[axis][AXIS]=value;
	printf("time: %ld axis:%d value:%d\n",sim_clock,axis,value);
	
}
void checkmotors(){
	/*1+ forward
	 *  20 1- backward
	 *   21 3+ all motors +
	 *    22 3- all motors - */
}
void status() {
	int pass=0;
	if(
	(sim_motors[MO_LEFT1] != sim_motors[MO_RIGHT1])||
	(sim_motors[MO_LEFT2] != sim_motors[MO_RIGHT2])) {
		printf("Mortors are fighting against eachother!!");
		bad=bad+1;//bad is not for failed test cases, it's extra
		pass=-1;
	}else {
		if(axis==2) { //forward backward
			if(
			sign(sim_motors[MO_LEFT1])==sign(sim_motors[MO_RIGHT1]) && 
			sign(sim_motors[MO_LEFT1])==sign(value)) { 
				pass=1;
			}else 
				pass=-1;

		}
	
		if(axis==1) { //left right
			if(
			(sign(sim_motors[MO_LEFT1])!=sign(sim_motors[MO_RIGHT1])||(value==0&&sim_motors[MO_RIGHT1]==0)) && 
			sign(sim_motors[1])==sign(value)) {
				pass=1;
			}else 
				pass=-1;
			
		}
	}//motor fight else
	printf("Good: %d Bad: %d Drive Motors: %d %d %d %d \n ",good,bad,
	sim_motors[MO_LEFT1],
	sim_motors[MO_LEFT2],
	sim_motors[MO_RIGHT1],
	sim_motors[MO_RIGHT1]);
	if(pass>=0){
		printf("\tPass\n");
		good=good+1;
	}
	else
		printf("\tFail\n");
}
void sim_housekeeping() {
	iterations+=1;
	status();
	sim_nextjoy();
	if(sim_clock>SIMTIME){
		die("done");
	}
}
int main() {
	printf("%d",MO_LEFT1);
	printf("testing...\n");

	operatorControl();
	return 0;
}
