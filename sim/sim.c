#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <linux/joystick.h>
#include <fcntl.h>
#include <sim.h>
#include <stdbool.h>
#include "./API.h"
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <sched.h>
long sim_clock=0;
void sim_die(char *s) {
	printf(s);
	exit(1);
}
void sim_delay(int ms) {
	//usleep(ms*10000);
	sim_clock=sim_clock+ms;
}
void motorSet(int channel, int speed) {
	if(channel<11)
		sim_motors[channel]=speed;
}
int joystickGetAnalog (unsigned char joystick, unsigned char axis) {
	return joy[axis][AXIS]/100;
}
int analogRead(int channel) {
	return analogs[channel];
}
bool joystickGetDigital (unsigned char joystick, unsigned char axis, unsigned char group) {
	return joy[axis+(group*4)][BUTTON]==0; 
}
void delay(int t) {
	sim_delay(t);
	sim_housekeeping();
}
struct params{ TaskCode t; void *p;};
int taskjump(void *pa)
{
	struct params *p=(struct params*)pa;
	p->t(p->p);
	return 0;

}
//holly fork batman! (I miss rfork...)
TaskHandle taskCreate(TaskCode taskCode, const unsigned int stackDepth, void *parameters, const unsigned int priority)
{
	//we could use the priority as a nice value
	void *stack=calloc(1,stackDepth);
	stack=stack+stackDepth;//stack grows from top
	struct params *p=calloc(sizeof(struct params),1);
	p->t=taskCode;
	p->p=parameters;
	int flags;
//	flags=SIGCHLD | CLONE_FILES | CLONE_FS | CLONE_IO | CLONE_VM;
	flags=CLONE_THREAD;

	if(taskCode){
		clone(taskjump,stack,flags,p);
	}
	return NULL;//TODO: return something sensible

}

void taskDelay(int n)
{
	usleep(100*n);

}
