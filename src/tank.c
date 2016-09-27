#include<robot.h>
#include<tank.h>
#include<math.h>
#ifdef SIM
#include<stdio.h>
#else
#include <API.h>
#include <main.h>
#endif
double f_tile_len=20;
#define f_center_x f_tile_len
#define f_center_y 2*f_tile_len
#define f_shot_x   f_tile_len*2
#define f_shot_y   f_tile_len*3
#define ROBOT_WIDTH 7.5
#define WEEL_WIDTH 4
double na(double t) {//normalize angle
	return fmod(t,2*PI);
}
void rotate(tank *v, double dh) {
	v->h=v->h-dh;
	v->h=na(v->h);
	
}
void simtank(tank *v,int lb, int lf, int rb, int rf) {
	//the change in heading is the difference of the speeds
	//the change in location is the sum of the two as the hypotonuse of the triangle where heading is the angle
	//except that the motors are flipped in polarity so invert all that
	//except we now the encoders are flipped so we don't
	double dx,dy,dh;
	//if(fabs(lspeed-rspeed)>100)
//		v->h=v->h+((lspeed-rspeed))/33.0;//TODO: use math
	
	dh=(lb+lf+rb+rf)/4;
	dx=(-lb+lf-rb+rf)/4;
	dy=(lb+lf-rf-rb)/4;
	rotate(v,dh);

	v->x=v->x+(dy*cos(v->h));
	v->y=v->y+(dy*sin(v->h));
	v->x=v->x+(dx*cos(v->h-(PI/2)));
	v->y=v->y+(dx*sin(v->h-(PI/2)));
}
double distanceto(tank v, double x, double y) {
	return sqrt(pow((v.x-x),2)+pow((v.y-y),2));
}
double headingto(tank v, double x, double y) {//return heading to target from current position
	/*
	//     robotpositionvector + newoffsetvector = targetpositionvector
	//=>   robotpositionvector - targetpositionvector = -newoffsetvector
	//=> -(robotpositionvector - targetpositionvector) = newoffsetvector
	//=>  -robotpositionvector + targetpositionvector = newoffsetvector
	double nx,ny;
	nx=-v.x+x;
	ny=-v.y+y;
	return 	atan2(nx,ny);
	*/ //that's retarded

	return asin(
			(sqrt(pow(x,2)+pow(y,2))*sin(atan2(x,y)-atan2(v.x,v.y)))
			/distanceto(v,x,y)
		   );
}
void printpos(tank *v) {
//	printf("POSITION: x:%f\t y:%f\t HEAD: %f\t tdist: %f\t thead:%f\t ",v->x,v->y,v->h,distanceto(*v,x,y),headingto(*v,x,y));
	printf("POSITION: x:%f\t y:%f\t HEAD: %f\t ",v->x,v->y,v->h);
}
/*
void driveto(tank v, double x, double y) {
	//double turn=(headingto(v,x,y)-v.h)*10;//This is wrong
	double turn=0;
	if(headingto(v,x,y)-0.5 >v.h)
		turn=30;
	if(headingto(v,x,y)+0.5 <v.h)
		turn=-30;
	//printf("%f ",turn);
	//printf("HEADINGTOTARGET:%f\n\r",headingto(v,x,y));
	int forward=ceil((distanceto(v,x,y)*30));
	if(forward >35)
		forward=35;
	//forward=0;
	controldrive(turn,forward);
}
void b_driveto(tank *v,double x, double y, double r) {
	while(distanceto(*v,x,y)>r) {
		//printf("DISTANCE: %f\n\r",distanceto(v,x,y));
		driveto(*v,x,y);
//		delay(20);
	}
}*/
void drivetogoal(tank *v) {
	//drive near center of map then use elijah line follow code + range sensor
	//reset position to in front of goal
	b_driveto(v,f_center_x,f_center_y,20);
//	elf();
}

