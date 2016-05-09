#include "../include/ir.h"
#include "../include/API.h"
int recive_bit();
unsigned int recive_byte();

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
	while(digitalRead(RX_PIN))printf("start");
	while(!digitalRead(RX_PIN)){
		c=c+1;
		delayMicroseconds(52);
		
	}
	printf("stop");
	return c/CYCLES_MULTIPLE;

}

