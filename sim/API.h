#include <stdbool.h>
#include <stdio.h>
void motorSet(int channel, int speed); 
int joystickGetAnalog (unsigned char joystick, unsigned char axis);
bool joystickGetDigital (unsigned char joystick, unsigned char axis, unsigned char group); 
void delay(int t); 
#define JOY_UP 1
