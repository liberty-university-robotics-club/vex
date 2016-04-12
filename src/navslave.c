#include "../include/ir.h"
#include "../include/API.h"
/*void setup() {
	// initialize digital pin 13 as an output.
	pinMode(RX_PIN, INPUT);
	Serial.begin(9600);
}*/

void loop()
{
	// put your main code here, to run repeatedly:
	//Serial.println(recive_byte());
	int a=rx_ec();
	if(a>1)
		printf("%d\n",a);
	//delay (200);


}
