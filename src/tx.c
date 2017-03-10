#include "../include/ir.h"
#include "../include/API.h"
#define POWER_PIN 6
void transmit_cycles(int cycles, int send_signal)
{
  int i=0;
	//send the cycles
	for(i=0;i<cycles;i++)
	{
    if(send_signal)digitalWrite(tx_pin, HIGH);
		delayMicroseconds(13);
		digitalWrite(tx_pin, LOW);
		delayMicroseconds(13);
	}
	//wait ten cycles before transmitting
	for(i=0;i<20;i++)
	{
		delayMicroseconds(13);
	}
}
void transmit_n(int n, unsigned int *data)
{//untested
	if(n>sizeof(int)*8)
		n=sizeof(int)*8;
	int j=0;
	for(;j<n;j++)
	{

		int mybit = *data%2;
		*data = *data/2;
		if(mybit)
		{
			transmit_cycles(CYCLES_TRUE, true);
		}
		else
		{
			transmit_cycles(CYCLES_FALSE, true);
		}
	}
}
void transmit_ob(unsigned int id, unsigned int val)
{
	unsigned int check=checkgen(id,val);
	transmit_cycles(CYCLES_HEADER, true);
	transmit_n(ID_LEN,&id);
	transmit_n(DATA_LEN,&val);
	transmit_n(CHECK_LEN,&check);
	transmit_cycles(CYCLES_HEADER, false);
}
void transmit_byte(unsigned int data)//little endian transmission of a byte 
{
/*	const int CYCLES_MULTIPLE = 23;
	const int CYCLES_HEADER = CYCLES_MULTIPLE*4;
	const int CYCLES_TRUE = CYCLES_MULTIPLE*2;
	const int CYCLES_FALSE = CYCLES_MULTIPLE;*/

	transmit_cycles(CYCLES_HEADER, true);

	int j=0;
	for(;j<8;j++)
	{
	  //int mybit = data<<j>>7;//doesn't work

    int mybit = data%2;
    data = data/2;
    
		if(mybit)
		{
			transmit_cycles(CYCLES_TRUE, true);
		}
		else
		{
			transmit_cycles(CYCLES_FALSE, true);
		}
	}
	
	transmit_cycles(CYCLES_HEADER, false);

  delay(5);
}

