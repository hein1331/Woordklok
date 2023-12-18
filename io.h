#ifndef IO_H
#define	IO_H

#include <xc.h>
#include "stdtypes.h"

//IO Definitions
#define INPUT       1
#define OUTPUT      0

#define DIGIO       0
#define ANLIO       1


//Periphial Pin Selection (PPS)
#define SDA_1       0x10;
#define SCL_1       0x0F;

#define UART_TX_1   0b1001;
#define UART_RX_1   0b1010;

#define UART_TX_2   0b1011;
#define UART_RX_2   0b1100;

//IOC Flags
#define MIN_INT         IOCAFbits.IOCAF0
#define AUD_JACK_INT    IOCAFbits.IOCAF5
#define PROG_SWIT_INT   IOCCFbits.IOCCF2

//Output IO
#define STAT_R      PORTAbits.RA7
#define STAT_G      PORTAbits.RA6
#define STAT_B      PORTCbits.RC0
#define STAT_BT     PORTCbits.RC1
#define SBD_EN_1    PORTBbits.RB4
#define SBD_EN_2    PORTBbits.RB3
#define SBD_EN_3    PORTBbits.RB2

//Functions
void init_io(void);
void set_io(IO io, STAT stat);
void prog_sw_process(void);

#endif	/* IO_H */

