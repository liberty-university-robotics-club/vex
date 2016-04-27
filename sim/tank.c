#include<API.h>
#include<tank.h>
void simtank(tank *v,int dl, int dr) {
	//the change in heading is the difference of the speeds
	//the change in location is the sum of the two as the hypotonuse of the triangle where heading is the angle
	//except that the motors are flipped in polarity so invert all that
	//except we now the encoders are flipped so we don't
	double lspeed=dl*0.1;
	double rspeed=dr*0.1;
	double r=lspeed+rspeed;
	//if(fabs(lspeed-rspeed)>100)
		v->h=v->h+(lspeed-rspeed);

	v->x=v->x+(r*cos(v->h));
	v->y=v->y+(r*sin(v->h));
}
void printpos(tank *v) {
	printf("POSITION: x:%f\t y:%f\t HEAD: %f",v->x,v->y,v->h);
}
