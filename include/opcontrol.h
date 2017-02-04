#ifndef OPC
#define OPC

void controldrive(int t, int f, int s);
void start_auto_lift(bool go_up);
bool continue_auto_lift();
int auto_lift(bool begin_auto, bool go_up);

#define DONT_MOVE 0

#endif
