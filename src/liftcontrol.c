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

bool get_lift()
{
	return digitalRead(LIFT_PIN);
}

void manage_lift()
{
	static int 	timer 		= 0;
	bool 		button_down = joystickGetDigital(1, 8, JOY_LEFT);
	
	if (timer == 0)
	{
		if (button_down)
		{
			set_lift(true);
			timer = 25;
		}else{
			set_lift(false);
		}
	}else{
		timer--;
	}	
}

