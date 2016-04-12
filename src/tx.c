#include "../include/ir.h"
#include "../include/API.h"
#define POWER_PIN 6
void transmit_cycles(int cycles, int send_signal)
{
  int i=0;
	//send the cycles
	for(i=0;i<cycles;i++)
	{
    if(send_signal)digitalWrite(13, HIGH);
		delayMicroseconds(13);
		digitalWrite(13, LOW);
		delayMicroseconds(13);
	}
	//wait ten cycles before transmitting
	for(i=0;i<20;i++)
	{
		delayMicroseconds(13);
	}
}

void transmit_byte(unsigned char data)//little endian transmission of a byte 
{
/*	const int CYCLES_MULTIPLE = 23;
	const int CYCLES_HEADER = CYCLES_MULTIPLE*4;
	const int CYCLES_TRUE = CYCLES_MULTIPLE*2;
	const int CYCLES_FALSE = CYCLES_MULTIPLE;*/
	int i=0;

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

