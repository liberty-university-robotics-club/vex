#include "../include/ir.h"
#include "../include/API.h"
int recive_bit();
unsigned int recive_byte();
int recive_ob(unsigned int *,unsigned int *);
unsigned int recive_n(int);


int recive_ob(unsigned int *id, unsigned int *ob){
	*id=recive_n(ID_LEN);
	*ob=recive_n(DATA_LEN);
	unsigned int check=checkgen(*id,*ob);
	unsigned int rcheck=recive_n(CHECK_LEN);
	if(rcheck==check)
		return 1;
	return 0;
}
unsigned int recive_n(int n) {
	int i=0;
	unsigned int r=0;
	while(recive_bit()<2);
	for(i=0;i<n;i++){
		r=r|(recive_bit()<<i);
	}
	return r;

}
unsigned int recive_byte() {
	int i=0;
	unsigned int r=0;
	while(recive_bit()<2);
	/*for(i=0;i<7;i++){
		r=r|(recive_bit());
		r=r<<1;
	}*/
	for(i=0;i<8;i++){
		r=r|(recive_bit()<<i);
		//r=r<<1;
	}
	return r;

}
int recive_bit() {
	int c=0;
	while(digitalRead(RX_PIN))delayMicroseconds(10);
	while(!digitalRead(RX_PIN)){
		c=c+1;
		delayMicroseconds(52);
		
	}
	return c/CYCLES_MULTIPLE;
}

