#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#include "stdtypes.h"

//Flags
#define PWM_FLAG        PIR0bits.TMR0IF //Cleared by SW
#define IOC_FLAG        PIR0bits.IOCIF //Cleared by SW, IOCIF is a read-only bit, to clear the interrupt condition, all bits in the IOCF register must be cleared.
#define ACD_FLAG        PIR1bits.ADIF //Cleared by SW
#define USB_REC_FLAG    PIR3bits.RC2IF //cleared by reading RC2REG
#define BT_REC_FLAG     PIR3bits.RC1IF //cleared by reading RC1REG

void init_interrupt(void);

#endif	/* INTERRUPTS_H */

