#include "main.h"
#include "ir.h"
void autonomous() {
#ifdef ALEN
#pragma message ("building auto for Alen")
	printf("Start Alen auto....");
	while(1) slave_loop(); //TODO: user interupts
#endif
}
