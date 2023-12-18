/* 
 * File:   stdtypes.h
 * Author: heinrich
 *
 * Created on September 17, 2017, 1:51 PM
 */

#ifndef STDTYPES_H
#define	STDTYPES_H

//Define CPU speed
#define _XTAL_FREQ 64000000

typedef int     IO;
typedef int     STAT;
typedef int     BOOL;

//#define ON      1
#define OFF     0

#define TRUE    1
#define FALSE   0

#define ENABLED    1
#define DISABLED   0


//Communication
typedef char            SLAVE_ADDR;
typedef char            REG_ADDR;
typedef int             SERIAL_SYS;
#define MAX_STR_CMD     50

//Timing
#define FREQ            64.E6f
#define LED_REF_RAT     10
#define MAX16BIT        65535


#endif	/* STDTYPES_H */

