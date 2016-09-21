#include "../include/controlloop.h"

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
