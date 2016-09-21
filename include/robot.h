#ifndef robot_h
#define robot_h

// MOTOR PINS

#define MLB 2
#define MLF 3
#define MRB 4
#define MRF 5

#define FORWARD_lf = 1  
#define FORWARD_lb = 1  
#define FORWARD_rf = -1 
#define FORWARD_rb = -1 

#define SIDEWAYS_lf = 1 
#define SIDEWAYS_lb = -1
#define SIDEWAYS_rf = 1 
#define SIDEWAYS_rb = -1



// SENSOR PINS

#define ENC_LBT 3
#define ENC_LBB 4
#define ENC_LFT 5
#define ENC_LFB 6
#define ENC_RBT 7
#define ENC_RBB 8
#define ENC_RFT 9
#define ENC_RFB 10

// CONTROLLER PINS

#define JOY_FORWARD_T  2
#define JOY_FORWARD_S  3
#define JOY_TURN_T     1
#define JOY_STRAFING_S 4

// THRESHOLDS AND CONSTANTS

#define JOY_DEAD_T 20

#endif
