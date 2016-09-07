#include "main.h"
#include "robot.h"
#include "ir.h"
#define TASK_STACK_D 70 
#define IR_PRIORITY TASK_PRIORITY_HIGHEST-2
#define MAIN_PRIORITY TASK_PRIORITY_HIGHEST-5
void ir_task (void * t) {
#ifdef ALEN
	printf("Start Alen IR...."); //TODO: atomic?
	while(1){
		slave_loop(); //TODO: user interupts
		taskDelay(10);
	}
#endif
}
void alen_auto(void *t){
	while(1){
		printf("Alen main loop\n\r");
		taskDelay(300);
	}
}
void autonomous() {
#ifdef ALEN
#pragma message ("building auto for Alen")
	printf("Start Alen auto....");
	taskCreate(ir_task,TASK_STACK_D,NULL,IR_PRIORITY);
	taskCreate(alen_auto,TASK_STACK_D,NULL,MAIN_PRIORITY);
	while(1)delay(1000);
#endif
}
