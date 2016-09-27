#ifndef LIFTCONTROL_H_INCLUDED
#define LIFTCONTROL_H_INCLUDED


#include <stdbool.h> 

//move this into robot.h sometime
#define LIFT_PIN 11


void init_lift();

void set_lift(bool value);

#endif
