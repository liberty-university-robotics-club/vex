#ifndef OPC
#define OPC

void controldrive(int t, int f, int s);
//void start_auto_lift(bool go_up);
//bool continue_auto_lift();
//int auto_lift(bool begin_auto, bool go_up);
//void start_auto_claw(bool clench);
//bool continue_auto_claw();
//int auto_claw(bool begin_auto, bool clench);

void lift_arm(int lift_pow);
void stop_claw();
void raise_fork(int hoist_pow);

void open_claw();
void close_claw();
int waited(int ms);
int waited2(int ms);
int waited3(int ms);


#define DONT_MOVE 0

#endif
