#ifndef CONTROLLOOP_H_INCLUDED
#define CONTROLLOOP_H_INCLUDED

typedef struct state
{
	int speed;
} state ;

/***********************************************************************
 * Causes the robot to move in a straight line (we hope.)
 * @param current: Encoder value
 * @param target: uncorrected power values
 * @param st: state struct pointer to last 
 **********************************************************************/
int controlLoop(int current, int target, state* st);

#endif // CONTROLLOOP_H_INCLUDED
