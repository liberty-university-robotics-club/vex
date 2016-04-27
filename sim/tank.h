#include <math.h>
#define PI 3.14159
extern void autonomous(void);
typedef struct tank {
	double x,y,h;
	double width;//not necesary?
} tank;
void sim_setsensors(tank *v);
void simtank(tank *v,int dl, int dr);
void printpos(tank *v);


