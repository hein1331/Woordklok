
#include "stdtypes.h"

#ifndef PWM_H
#define	PWM_H



#define TIM0FREQ    5000//Hz

#define PWM_INT     PIR0bits.TMR0IF

typedef int     CLK_STAT;
enum
{
    ERROR,
    INIT,
    BADTQ,
    TIMSET, 
    CLOCKMD,
    AUDMD
};

//PWM Colors
//Red: R:10/10, G:0/10, B:0/10
#define RED         r_cnt = 10; g_cnt = 0; b_cnt = 0;

//Orange: R:6/10, G:4/10, B:0/10
#define ORANGE      r_cnt = 7; g_cnt = 3; b_cnt = 0;

//Purple: R:4/10, G:0/10, B:6/10
#define PURPLE      r_cnt = 4; g_cnt = 0; b_cnt = 6;

//Green: R:0/10, G:10/10, B:0/10
#define GREEN       r_cnt = 0; g_cnt = 10; b_cnt = 0;

//Blue: R:0/10, G:0/10, B:10/10
#define BLUE        r_cnt = 0; g_cnt = 0; b_cnt = 10;

//Yellow: R:5/10, G:5/10, B:0/10
#define YELLOW        r_cnt = 5; g_cnt = 5; b_cnt = 0;


//Bluetooth LED Blink
//Blinking rate of BT LED must be greater than or equal to 1 Hz
const int BTCNTMAX  = (int)((float)TIM0FREQ/2.0/*Hz*/);
const int BTBLINK   = (int)((float)BTCNTMAX * 0.5f);


//Functions
void init_pwm(void);
void pwm_process(void);
void set_status_led(CLK_STAT status, BOOL error_override);
void set_bt_led(STAT stat, BOOL blinking);
void set_error_state(BOOL error);

#endif	/* PWM_H */

