#include "pwm.h"
#include "io.h"

volatile int r_cnt, curr_r_cnt;
volatile int g_cnt, curr_g_cnt;
volatile int b_cnt, curr_b_cnt;

volatile int bt_cnt, curr_bt_cnt;

//Testing
volatile BOOL toggle;

volatile CLK_STAT curr_stat;

void init_pwm(void)
/*Init PWM for the status and bluetooth LED*/
{
    //Testing 
    toggle = FALSE;      
           
    //Init count variables
    r_cnt = 0;
    g_cnt = 0;
    b_cnt = 0;
    bt_cnt = 0;
    
    curr_r_cnt = 0;
    curr_g_cnt = 0;
    curr_b_cnt = 0;
    curr_bt_cnt = 0;
            
    //Init timer 0 at 20kHz
    T0CON0bits.T0EN = DISABLED; //Disable timer
    T0CON0bits.T016BIT = FALSE; //8 bit counter
    T0CON0bits.T0OUTPS = 0b0000; //1:1 Postscalar
    T0CON1bits.T0CS = 0b011; //Clock source is HFINTOSC @ 64MHz
    T0CON1bits.T0ASYNC = TRUE; //Do not sync to system clock
    T0CON1bits.T0CKPS = 0b0110; //Prescalar 1:64
    TMR0H = 199; //Count 50 times before reset
    TMR0L = 0; //Reset counter
    
    T0CON0bits.T0EN = ENABLED; //Start the timer. Will not trigger interrupt
    
    curr_stat = INIT;
}


void pwm_process(void)
/*Process PWM of the LEDs*/
{
    
    //Process status LED PWM
    STAT_R = ~(curr_r_cnt-- > 0);
    STAT_G = ~(curr_r_cnt < 0 && curr_g_cnt-- > 0);
    STAT_B = ~(curr_r_cnt < 0 && curr_g_cnt < 0 && curr_b_cnt-- > 0);
    
    //Reset values to count
    if(curr_r_cnt <= 0 && curr_g_cnt <= 0 && curr_b_cnt <= 0)
    {
        curr_r_cnt = r_cnt;
        curr_g_cnt = g_cnt;
        curr_b_cnt = b_cnt;
    }
    
   
    //Process bluetooth LED
    STAT_BT = ~(curr_bt_cnt++ < bt_cnt);
    curr_bt_cnt = curr_bt_cnt * (curr_bt_cnt < BTCNTMAX);
  
    PWM_INT = FALSE;
    
}


void set_status_led(CLK_STAT status, BOOL error_override)
/*Sets the color of the status LED based on the status*/
{
    //Do not allow error override
    if(curr_stat == ERROR && !error_override)
        return;
    
    switch(status)
    {
        case INIT:
            YELLOW
            break;
        case BADTQ:
            ORANGE
            break;
        case TIMSET:
            PURPLE
            break;
        case CLOCKMD:
            GREEN
            break;
        case AUDMD:
            BLUE
            break;
        default: //ERROR
            RED
            break;       
    }
}


void set_bt_led(STAT stat, BOOL blinking)
/*Sets the bluetooth LED to on, off, blinking*/
{
    if(stat && blinking)
        bt_cnt = BTBLINK;
    else if(stat)
        bt_cnt = BTCNTMAX;
    else
        bt_cnt = 0;   
}


void set_error_state(BOOL error)
{
    if(error)
    {
        curr_stat = ERROR;
        set_status_led(ERROR, TRUE);
    }else
    {
        curr_stat = CLOCKMD;
        set_status_led(CLOCKMD, TRUE);
    }
    
    
    
}