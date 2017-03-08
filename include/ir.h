#ifndef ir_h
#define ir_h
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
int recive_ob(unsigned int *id, unsigned int *ob);

//transmit
void transmit_byte(unsigned int);
void tx_ec(unsigned int);
void transmitob(unsigned int id, unsigned int val);
//check
unsigned int checkgen(unsigned int, unsigned int);
//interface to main auto code
void slave_loop(void);
void master_loop(void);
#endif
