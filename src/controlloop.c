#include "../include/controlloop.h"
#include <math.h>

//TODO: normalise T:
// m ( e + (64/(t_j + e_j))t)
//where j is max_i(|e_i|)
//this will slow the robot down when theere is large error between motors so that they wont clip

//TODO: somehwere else, scale p (current) so it's in same range as target
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

