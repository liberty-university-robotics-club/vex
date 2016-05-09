#ifndef ir_h
#define ir_h
#define RX_PIN 2
#define CYCLE_LEN 13
#define CYCLES_MULTIPLE  23
#define CYCLES_HEADER  CYCLES_MULTIPLE*4
#define CYCLES_TRUE CYCLES_MULTIPLE*2
#define CYCLES_FALSE  CYCLES_MULTIPLE
//recive
unsigned int recive_byte(void);
unsigned int rx_ec(void);

//transmit
void transmit_byte(unsigned int);
void tx_ec(unsigned int);
//interface to main auto code
void slave_loop(void);
void master_loop(void);
#endif
