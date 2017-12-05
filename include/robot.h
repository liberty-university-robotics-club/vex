#ifndef robot_h
#define robot_h

//#define GODDARD
#define IMP

// CONTROLLER PINS + CONSTANTS

#define JOY_FORWARD_T  2
#define JOY_FORWARD_S  3
#define JOY_TURN_T     1
#define JOY_STRAFING_S 4

#define JOY_LIFT_OP    5
#define JOY_LIFT       6


// MOTOR PINS + CONSTANTS

#define MLB 2			//
#define MLF 3			//
#define MRB 4			//
#define MRF 5			//

#ifdef IMP
#define MLIFT 8			//
#define MLIFT_POW 127
#endif

#ifdef GODDARD
#define MHOIST 8		//
#define MHOOK 9			//
#define HOIST_POW 127
#define HOOK_POW 127
#endif



#define FORWARD_lb 1
#define FORWARD_lf 1
#define FORWARD_rb -1
#define FORWARD_rf -1

#define SIDEWAYS_lb -1
#define SIDEWAYS_lf 1
#define SIDEWAYS_rb -1
#define SIDEWAYS_rf 1


// SENSOR PINS + CONSTANTS

#define ENC_LBT  1
#define ENC_LBB  2
#define ENC_LFT  3
#define ENC_LFB  4
#define ENC_RBT  5
#define ENC_RBB  6
#define ENC_RFT  7
#define ENC_RFB  8
#define BTN_UP   9
#define BTN_DWN 10

#define IME_IR 0

#define US_portPing 10
#define US_portEcho 9

#define TARGET_POW 64
#define POS_POW 64
#define TARGET_DIST 20
#define CONE_DELTA 10
#define FAR_DIST 100
#define SORTA_CLOSE 50
#define LIFT_TIMER 20
#define CLAW_TIMER 20
#define CLAW_JOY 8

#define CLAW_PIN 12

void* ENC_LB;
void* ENC_LF;
void* ENC_RB;
void* ENC_RF;


// OTHER THRESHOLDS AND CONSTANTS

#define DELAY_ms 25
#define LIFT_TIMEOUT_s 2.5 //float
#define CLAW_TIMEOUT_s 3.5 //float
//#define HOIST_PERIOD_s .25 //float


#define JOY_DEAD_T 40
#define SCALED_SPEED_CONVERSION 1.0
//#define MAGIC_NUMBER 20

#define ID_THETA 0

#endif
