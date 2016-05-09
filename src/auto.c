#include "main.h"
#include "ir.h"
void autonomous() {
#ifdef ALEN
#pragma message ("building auto for Alen")
	slave_loop(); //TODO: user interupts
#endif
}
