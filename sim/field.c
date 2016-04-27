#include <sim.h>
#include <tank.h>
#include <math.h>
typedef struct tapeline {
	double x1;
	double x2;
	double y1;
	double y2;
}tapeline;
typedef struct vd2 {
	double x;
	double y;
}vd2;
tapeline field[8];
int lcount;
double ldist(vd2 v,tapeline l){//TODO:measure distance from segment, not infinit line
	double m=(l.y2-l.y1)/(l.x2-l.x1);
	double b;
	if (l.x1!=0)
		b= l.y1/(m*l.x1); //y=mx+b => y/mx=b
	else
		 b=0;
	//
	//distance function: d^2=(x2-x1)^2 + (y2-y1)^2
	//the distance between a  functin and a point will be the min [ distance(pt,f(x))] 
	
	double r= (v.x - m*b +v.x*m*v.y)/(m*m+1);//Thanks Elijah
	printf("distance: %f\n\n",r);
	
	return r;
}
void initfield() {//field map, the field is made of 2ft^2 mats that interlock, it is 6x6 mats with the origin at the lower left corner (red base)
	//TODO: revise this!
	//the field is symetrical with two lines crossing eachother in the center andlines sourounding each 4ft^2 base with a single line diaganoly going through the center from the corner of the field.
	
	field[0].x1=0;
	field[0].x2=6;
	field[0].y1=0;
	field[0].y2=6;
	field[1].x1=6;
	field[1].x2=0;
	field[1].y1=0;
	field[1].y2=6;
	lcount=2;
}
vd2 rtransv (vd2 v, double a, vd2 t){
	vd2 r;
	r.x=(cos(a)*v.x) -(cos(a)*v.y)+t.x;
	r.x=(sin(a)*v.x) +(cos(a)*v.y)+t.y;
	//r.x=(cos(a)*v.x) -(cos(a)*v.y);
	//r.x=(sin(a)*v.x) +(cos(a)*v.y);
	return r;
}

void sim_setsensors(tank *v) {
	v->battery=v->battery-0.0001;
	vd2 lv,tv;
	tv.x=v->x;
	tv.y=v->y;
	int i;
	for(i=0;i<lcount;i++){
		lv.x=0;lv.y=v->lsoffset;
		analogs[1]=ldist(rtransv(lv,v->h,tv),field[i])<0.1 ? 1000 :0;
		lv.x=-v->lswidth;lv.y=v->lsoffset;
		analogs[0]=ldist(rtransv(lv,v->h,tv),field[i])<0.1 ? 1000 :0;
		lv.x=v->lswidth;lv.y=v->lsoffset;
		analogs[2]=ldist(rtransv(lv,v->h,tv),field[i])<0.1 ? 1000 :0;
	}
}
