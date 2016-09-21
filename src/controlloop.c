#include "../include/controlloop.h"

int controlLoop(int current, int target, state* st)
{
    int increase = 1;
    if (current < target)
    {
        return current += increase;
    }else
    {
        return current -= increase;
    }
}
