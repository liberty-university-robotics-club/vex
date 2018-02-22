#ifndef robot_h
#define robot_h


//Includes
#include "API.h"


#include "API.h"

//#define GODDARD
#define IMP

// CONTROLLER PINS + CONSTANTS

#define JOY_FORWARD_T  2
#define JOY_FORWARD_S  3
#define JOY_TURN_T     1
#define JOY_STRAFING_S 4

#define JOY_LIFT       5
#define JOY_CLAW       6
#define JOY_AUTO_TEST  7
#define JOY_OP_AUTO    8


// MOTOR PINS + CONSTANTS

#define MLB 2			
#define MLF 3			
#define MRB 4			
#define MRF 5			

#define MLIFTLB 6
#define MLIFTRB 7
#define MLIFTLT 8
#define MLIFTRT 9

#define MCLAWPIV 10
#define MCLAWP 11

#define MGLIFTL 1
#define MGLIFTR 12

#ifdef IMP
#define MLIFT 8			
#define MLIFT_POW 127
#endif

#ifdef GODDARD
#endif


// Ultrasonic Sensors
Ultrasonic US1;
Ultrasonic US2;


#define FORWARD_lb 1
#define FORWARD_lf 1
#define FORWARD_rb -1
#define FORWARD_rf -1

#define SIDEWAYS_lb -1
#define SIDEWAYS_lf 1
#define SIDEWAYS_rb -1
#define SIDEWAYS_rf 1


// SENSOR PINS + CONSTANTS

/*#define ENC_LBT  1
#define ENC_LBB  2
#define ENC_LFT  3
#define ENC_LFB  4
#define ENC_RBT  5
#define ENC_RBB  6
#define ENC_RFT  7
#define ENC_RFB  8*/
#define EMLB 1
#define EMLF 2
#define EMRB 3
#define EMRF 4

#define US_portEcho 9
#define US_portPing 10
#define CLAW_PIN 12

#define TARGET_POW 128 //64
#define POS_POW 64
#define TARGET_DIST 30 //20
#define CONE_DELTA 10
#define FAR_DIST 100
#define SORTA_CLOSE 50
#define LIFT_TIMER 20
#define CLAW_TIMER 20

#define POW_WHEELIE 127


void* ENC_LB;
void* ENC_LF;
void* ENC_RB;
void* ENC_RF;

Ultrasonic US;


// OTHER THRESHOLDS AND CONSTANTS

#define DELAY_ms 25
#define LIFT_TIMEOUT_s 2.5 //float
#define CLAW_TIMEOUT_s 3.5 //float
//#define HOIST_PERIOD_s .25 //float
#define WAIT_RESET -1


#define JOY_DEAD_T 40
#define SCALED_SPEED_CONVERSION 1.0
//#define MAGIC_NUMBER 20

#define ID_THETA 0

#endif
