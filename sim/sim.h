#include <stdbool.h>
#define AXIS 2
#define BUTTON 1
extern void operatorControl(void); //this is where the robot code you're testing goes
extern void sim_housekeeping(void); //this is where your test/ui code goes
int sim_joyfd;
int joy[11][2]; //larger then it neeeds to be
int sim_motors[11];
int analogs[11]; //TODO: how many are there?
extern long sim_clock;
void sim_delay(int ms); 
void sim_joyinit(); 
void sim_getjoyevents(); 
void sim_die(char *s);
