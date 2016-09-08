#include "../include/ir.h"
#include <math.h>
void tx_ec(unsigned int m){
	transmit_byte(m);
	transmit_byte(m);
	transmit_byte(m);
}

unsigned int rx_ec(){
	static unsigned int l=0;
	static unsigned int c=0;
	unsigned int b=recive_byte();
	if(l==b)
		c=c+1;
	else
		c=0;
	if(c>=1){
		c=0;
		l=0;
		return b;
	}
	l=b;
	return c;
}
unsigned int checkgen(unsigned int id, unsigned int val)
{
	return (id+val)%(int)pow(CHECK_LEN,2);
}
