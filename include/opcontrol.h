#ifndef OPC
#define OPC

void controldrive(int t, int f, int s);
void start_auto_lift(bool go_up);
bool continue_auto_lift();
int auto_lift(bool begin_auto, bool go_up);
void start_auto_claw(bool clench);
bool continue_auto_claw();
int auto_claw(bool begin_auto, bool clench);


#define DONT_MOVE 0

#endif
