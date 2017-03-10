#ifndef ir_h
#define ir_h
int tx_pin; //user changes this to send on different blasters
#define RX_PIN 2
#define CYCLE_LEN 13
#define CYCLES_MULTIPLE  23
#define CYCLES_HEADER  CYCLES_MULTIPLE*4
#define CYCLES_TRUE CYCLES_MULTIPLE*2
#define CYCLES_FALSE  CYCLES_MULTIPLE
//constants defining packets
#define ID_LEN 3
#define DATA_LEN 8
#define CHECK_LEN 3
//recive
unsigned int recive_byte(void);
unsigned int rx_ec(void);

//transmit
void transmit_byte(unsigned int);
void tx_ec(unsigned int);
//check
unsigned int checkgen(unsigned int, unsigned int);
//interface to main auto code
void slave_loop(void);
void master_loop(void);
int recive_ob(unsigned int *, unsigned int *);
void transmit_ob(unsigned int , unsigned int );

int gotflag;

//object ids 0-7
#define IR_ID_R 0
#define IR_ID_T 1

#endif
