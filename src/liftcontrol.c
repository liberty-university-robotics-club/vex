//pinMod(unsigned char pin, OUTPUT)
//digitalWrite(unsigned char pin, bool value)
//

#include "../include/liftcontrol.h"
#include "../include/API.h"

void init_lift()
{
	pinMode(LIFT_PIN, OUTPUT);
}

void set_lift(bool value)
{
	digitalWrite(LIFT_PIN, value);
}

