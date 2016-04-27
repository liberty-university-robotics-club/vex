#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <linux/joystick.h>
#include <unistd.h>
#include <fcntl.h>
#include <sim.h>
#include <stdbool.h>
#include <path.h>

void sim_ui()
{
	int i;
	//printf("\033[;f");//home cursor
	printf("\033[2J\n");
	puts("\nMotors:");
	for(i=0;i<11;i++) {
		printf("%d\t",sim_motors[i]);
	}
}
void sim_housekeeping()
{
	sim_ui();
	usleep(1000*100);
}
int main () {
	printf("running path tests\n");
	printf("\033[2J\n");
	sim_joyinit();
	arc10();
	return 0;
}
