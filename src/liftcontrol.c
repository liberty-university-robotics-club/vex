//pinMod(unsigned char pin, OUTPUT)
//digitalWrite(unsigned char pin, bool value)
//

#include "../include/liftcontrol.h"
#include "../include/API.h"
#include "../include/robot.h"

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
	static int 	timer = 0;
	bool button_down = joystickGetDigital(1, 8, JOY_LEFT);
	static int led_on = 0;

	static int ptimer = 0;



	if (ptimer == 0)	{
		printf("button_down: %d\nLED_ON: %d\n", button_down, led_on);
		ptimer = 50;
	}else{
		ptimer--;
	}

	if (timer <= 12) //shutoff lift halfway through cycle 0.25s
	{
		if (!button_down)
		{
			set_lift(false);
			led_on = 0;
		}
	}
	
	if (timer == 0)
	{
		if (button_down)
		{
			set_lift(true);
			timer = 25;
			led_on = 1;
		}	
	}else{
		timer--;
	}	
}

