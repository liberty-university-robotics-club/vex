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
//int abs (int x)
//{
//	return x>0?x:-x;
//}
