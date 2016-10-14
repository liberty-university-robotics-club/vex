#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <linux/joystick.h>
#include <unistd.h>
#include <fcntl.h>
#include <sim.h>
#include <stdbool.h>

void sim_joyinit() {
	sim_joyfd=open("/dev/input/js0",O_RDONLY|O_NONBLOCK);

	if(sim_joyfd >= 0)
		printf("joystick open\n");

	if(sim_joyfd < 0)
		printf("joystick failed to open\n");
}

void sim_getjoyevents() {
	struct js_event e;
	while(read (sim_joyfd, &e, sizeof(e))>0) {
		if(e.type <3){
			joy[e.number][e.type]=e.value;//evil joysticks can break this
		}
	}
}
void sim_ui()
{
	int i;
	//printf("\033[;f");//home cursor
	//printf("\033[2J\n");
	
	printf("joystick value: %d\n", sim_joyfd);

	if (sim_joyfd >= 0){
		puts("\nMotors:");
		for(i=0;i<11;i++) {
			printf("%d\t",sim_motors[i]);
		}
		puts("\nJoystick");
		for(i=0;i<5;i++){
			printf("%d\t",joy[i][AXIS]);
		}
		puts("\nButtons");
		for(i=0;i<11;i++){
			printf("%d\t",joy[i][BUTTON]);
		}
		printf("\n");//need this for Buttons to finish printing
	}
}
void sim_housekeeping()
{
	sim_ui();
	usleep(1000*100);
	sim_getjoyevents(); 
}
int main () {
	printf("\033[2J\n");
	sim_joyinit();
	initialize();
	operatorControl();
	return 0;
}
