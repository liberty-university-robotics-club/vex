#include <math.h>
#define PI 3.14159
extern void autonomous(void);
typedef struct tank {
	double x,y,h;
	double width;//not necesary?
} tank;
tank ltank;
void sim_setsensors(tank *v);
void simtank(tank *,int, int,int, int);
void printpos(tank *v);
double headingto(tank v, double x, double y);
double distanceto(tank v, double x, double y);
void driveto(tank v, double x, double y);
void b_driveto(tank *v, double x, double y, double r);
void drivetogoal(tank *v);
void rotate(tank *v, double dh);
void base_station_update(tank *v, double r, double theta);
void newdriveto(int,double,double,tank*);
void constrain(tank *,double,int);
#define EAST 0
#define NORTH 1
#define WEST 2
#define SOUTH 3
void face(tank *,int dir);

void lift_block(bool);
