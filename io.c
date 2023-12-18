#include "io.h"
#include "pwm.h"
#include <xc.h>
#include <pic18f27k40.h>

void init_io(void)
/*Init the inputs and outputs of the PIC*/
{
    //Init Digital Outputs
    STAT_R = 1;
    ANSELAbits.ANSELA7 = DIGIO;
    TRISAbits.TRISA7 = OUTPUT;
     
    STAT_G = 1;
    ANSELAbits.ANSELA6 = DIGIO;
    TRISAbits.TRISA6 = OUTPUT;
     
    STAT_B = 1;
    ANSELCbits.ANSELC0 = DIGIO;
    TRISCbits.TRISC0 = OUTPUT;
   
    STAT_BT = 1;
    ANSELCbits.ANSELC1 = DIGIO;
    TRISCbits.TRISC1 = OUTPUT;
    /*
    SBD_EN_1 = 0;
    ANSELBbits.ANSELB4 = DIGIO;
    TRISBbits.TRISB4 = OUTPUT;
    
    SBD_EN_2 = 0;
    ANSELBbits.ANSELB3 = DIGIO;
    TRISBbits.TRISB3 = OUTPUT;
    
    SBD_EN_3 = 0;
    ANSELBbits.ANSELB2 = DIGIO;
    TRISBbits.TRISB2 = OUTPUT;
    
    */
    //Init interrupt on change IO
    //Minute Interrupt
    ANSELAbits.ANSELA0 = DIGIO;
    TRISAbits.TRISA0 = INPUT;
    IOCAPbits.IOCAP0 = ENABLED;
    IOCANbits.IOCAN0 = DISABLED;
    /*
    //Bluetooth Connected Interrupt
    ANSELAbits.ANSELA3 = DIGIO;
    TRISAbits.TRISA3 = INPUT;
    IOCAPbits.IOCAP3 = ENABLED;
    IOCANbits.IOCAN3 = ENABLED;
    
    //LED Controller Interrupt
    ANSELAbits.ANSELA4 = DIGIO;
    TRISAbits.TRISA4 = INPUT;
    IOCAPbits.IOCAP4 = ENABLED;
    IOCANbits.IOCAN4 = DISABLED;
    */
    //Audio Jack Status Interrupt
    ANSELAbits.ANSELA5 = DIGIO;
    TRISAbits.TRISA5 = INPUT;
    IOCAPbits.IOCAP5 = ENABLED;
    IOCANbits.IOCAN5 = ENABLED;
    
    //Programming Switch Status Interrupt
    ANSELCbits.ANSELC2 = DIGIO;
    TRISCbits.TRISC5 = INPUT;
    IOCCPbits.IOCCP2 = ENABLED;
    IOCCNbits.IOCCN2 = ENABLED;
    /*
    
    //ADC IO Init
    //Right Audio ADC
    ANSELAbits.ANSELA1 = ANLIO;
    TRISAbits.TRISA1 = INPUT;
    */
    //Enable I2C Comm on Ports
    ANSELCbits.ANSELC3 = DIGIO;
    TRISCbits.TRISC3 = INPUT;
    ANSELCbits.ANSELC4 = DIGIO;
    TRISCbits.TRISC4 = INPUT;
    SSP1DATPPS = 0b10100; //SDA Input set to RC4
    SSP1CLKPPS = 0b10011; //SCL Input set to RC3
    RC3PPS = SCL_1;
    RC4PPS = SDA_1;
    
    //Serial Communication Ports Init
    //USB-Serial Communication
    RB7PPS = UART_RX_2;
    RB5PPS = UART_TX_2;
    ANSELBbits.ANSELB7 = DIGIO;
    TRISBbits.TRISB7 = INPUT;
    
    //Bluetooth Communication
    RC7PPS = UART_RX_1;
    RC6PPS = UART_TX_1;
    ANSELCbits.ANSELC7 = DIGIO;
    TRISCbits.TRISC7 = INPUT;
   
}


void set_io(IO io, STAT stat)
/*Set a digital input or output to stat*/
{
    io = stat;
}


void prog_sw_process(void)
{
    BOOL prog_sw_stat = PORTCbits.RC2;
    set_error_state(!prog_sw_stat);   
    PROG_SWIT_INT = FALSE;    
}