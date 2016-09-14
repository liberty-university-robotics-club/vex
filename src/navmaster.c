#include "../include/ir.h"
#include "../include/API.h"
#define SERVO_PIN 3
//Servo aimservo;
#define POWER_PIN 6
/*void setup() {
	// initialize digital pin 13 as an output.
	pinMode(13, OUTPUT);
	pinMode(POWER_PIN,OUTPUT);
	aimservo.attach(SERVO_PIN);

}
void aim(int v){
	static int cv=0;
	if(v>cv)
		cv+=2;
	else if(v<cv)
		cv-=2;
	aimservo.write(cv);
}
*/
void master_loop()
{
	// put your main code here, to run repeatedly:
	int i = 0;
//	const int min = 20;
//	const int max =120;
	/*for(i=0;i<256;i++)
	{
		aim(i);
		transmit_byte(i);
		delay(100);
	}*/
	while(1){
		/*for(i=min;i<max;i++){
			aim(i);
			tx_ec(i);
			delay(25);
		}
		for(i=max;i>min;i--){
			aim(i);
			tx_ec(i);
			delay(25);
		}*/
		for(i=110;i>60;i=i-1){
//			analogWrite(POWER_PIN,255-i); //TODO: fix analogWrite
			//170 is about 2 feet and the lower the value is the farther it reaches (This is due to the properties of the transistor led driver hardware and can be chagned by changing the hardware)
			tx_ec(i);
			delay(25);
		}
	}

	delay(1000);
}
