#include "../include/controlloop.h"
#include <math.h>

int controlLoop(int current, int target, state* st)
{
	int dist=fabs(current-target);
	if(dist<2)
		return target;
	if(current<target)
		return target+dist/2;
	else
		return target-dist/2;
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
