#ifndef LIFTCONTROL_H_INCLUDED
#define LIFTCONTROL_H_INCLUDED


#include <stdbool.h> 

void init_lift();

void set_lift(bool value);

bool get_lift();

void manage_lift();

#endif
