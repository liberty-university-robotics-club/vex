#include <stdio.h>
#include <stdlib.h>
//#include <linux/input.h>
//#include <linux/joystick.h>
//#include <unistd.h>
#include <fcntl.h>
#include <sim.h>
#include <stdbool.h>
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
