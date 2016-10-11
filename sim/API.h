#ifndef API_H
#define API_H 1
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
void motorSet(int channel, int speed); 
int joystickGetAnalog (unsigned char joystick, unsigned char axis);
bool joystickGetDigital (unsigned char joystick, unsigned char axis, unsigned char group); 
void delay(int t); 
#define JOY_UP 1
#define OUTPUT 1
//task
#define TASK_PRIORITY_DEFAULT 2
#define TASK_DEFAULT_STACK_SIZE 512
typedef void (*TaskCode)(void *);
typedef void *TaskHandle;
TaskHandle taskCreate(TaskCode taskCode, const unsigned int stackDepth, void *parameters, const unsigned int priority);
#endif
