#include "../include/controlloop.h"
#include <math.h>

int controlLoop(int current, int target, state* st)
{
 	int increase = 1;
	if (st)
    {
    	increase = st->speed;
    }
    int abs = target - current;
		
    //make positive if negative
    if (abs < 0)
    {
    	abs *= -1;
    }
		
	//attempt at thresholding
	if (abs < 1)
	{
		increase = abs;
	}

	if (current < target)
    {
        return current += increase;
    }else
    {
        return current -= increase;
    }
}
int abs (int x)
{
	return x>0?x:-x;
}
int elijah_controlLoop(int last_enc, int target, int last_power)
{
	int max_power = 105;
	target = abs(target)<=max_power?target:max_power*target/abs(target);
	int anticipated_enc = target;
	
	if(last_enc != 0)
	{
		target *= last_power/last_enc;
		target = abs(target)<=max_power?target:max_power*target/abs(target);
		return target;
	}
	else
	{
		return target;
	}
	
	
	
}
