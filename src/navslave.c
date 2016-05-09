#include "../include/ir.h"
#include "../include/API.h"
/*void setup() {
	// initialize digital pin 13 as an output.
	pinMode(RX_PIN, INPUT);
	Serial.begin(9600);
}*/

void slave_loop()
{
	// put your main code here, to run repeatedly:
	//Serial.println(recive_byte());
	int a=rx_ec();
//	int a=recive_byte();
	if(a>1)
		printf("%d\n",a);
//	taskDelay(100);//We are running at a very high priority 
	//so we need to yeild the cpu to other tasks
	//in the future we should use an ISR for watching the state of the recive pin


}
