#ifndef CONTROLLOOP_H_INCLUDED
#define CONTROLLOOP_H_INCLUDED

typedef struct state
{
	int speed;
} state ;

int controlLoop(int current, int target, state* st);

#endif // CONTROLLOOP_H_INCLUDED
