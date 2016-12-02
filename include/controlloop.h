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
int elijah_controlLoop(int last_enc, int target, int last_power);

#endif // CONTROLLOOP_H_INCLUDED
