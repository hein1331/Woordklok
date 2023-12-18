#include <pic18f27k40.h>

#include "interrupts.h"
#include "pwm.h"
#include "communication.h"
#include "adc.h"
#include "io.h"
#include "led.h"

void init_interrupt(void)
{
    //Disable all interrupts
    INTCONbits.GIE = DISABLED;
    
    //Disable Peripheal interrupts
    INTCONbits.PEIE = DISABLED;
    
    //Enable interrupt priority
    INTCONbits.IPEN = TRUE;
 
    //High priority interrupts
    //USB Serial Received Interrupt
    USB_REC_INT = FALSE;
    PIE3bits.RC2IE = ENABLED;
    
    //Bluetooth Serial Received Interrupt
    BT_REC_INT = FALSE;
    PIE3bits.RC1IE = ENABLED;
    /*
    //ADC Conversion Complete Interrupt
    ADC_INT = FALSE;
    PIE1bits.ADIE = ENABLED;
    */
    //PWM Interrupt
    PWM_INT = FALSE;
    PIE0bits.TMR0IE = ENABLED;
          
    
    //Low Priority Interrupts
    //Set IOC as low priority
    IPR0bits.IOCIP = 0;
    //Enable IOC
    PIE0bits.IOCIE = ENABLED;
            
    //Minute interrupt
    MIN_INT = DISABLED;

    //LED Update Interrupt
    //SCR_UPD_INT = DISABLED;
    //PIE4bits.TMR2IE = ENABLED;

    //Audio Jack Status Interrupt
    AUD_JACK_INT = DISABLED;

    //Programming Switch Interrupt
    PROG_SWIT_INT = DISABLED;

    //Enable all interrupts
    INTCONbits.GIE = ENABLED;
    
    //Enable Peripheal interrupts
    INTCONbits.PEIE = ENABLED;
    
}
