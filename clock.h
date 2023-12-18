/* 
 * File:   clock.h
 * Author: heinrich
 *
 * Created on September 19, 2017, 6:06 PM
 */

#ifndef CLOCK_H
#define	CLOCK_H

#include "stdtypes.h"

typedef struct
{
    unsigned int second;
    unsigned int minute;
    unsigned int hour;
    unsigned int day;
    unsigned int month;
    unsigned int year;
} TIMEDAT;
    
    
#define TIM_QLTY_RST        262800

#define VAL_TO_BCD(val)     (((val - (val%10))/10)<<4)+(val%10)
#define BCD_TO_VAL(val)     (unsigned int)(((val>>4)*10)+(val & 0xF)) 


//Functions
void init_clock(void);
BOOL set_time(TIMEDAT *set_time);
TIMEDAT *get_time(void);
BOOL time_needs_to_be_set(void);

void wait_for_clock_stable(void);

#endif	/* CLOCK_H */

