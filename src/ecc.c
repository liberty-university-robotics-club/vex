#include "../include/ir.h"
void tx_ec(int m){
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
	if(c>=2){
		c=0;
		l=0;
		return b;
	}
	l=b;
	return c;
}
