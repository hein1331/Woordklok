#include "clock.h"
#include "communication.h"
#include "io.h"

volatile unsigned long time_quality;
TIMEDAT curr_time;

void init_clock(void)
{
   
    wait_for_clock_stable();
    //Disable timer and detach INT from bit
    i2c_write_single(0b00011001, CLOCK_CHIP, 0x11);
    __delay_us(100);
    //Enable minute interrupt and disable clock output
    i2c_write_single(0b00100111, CLOCK_CHIP, 0x01);
    time_quality = 0;
}


void wait_for_clock_stable(void)
{
    char l = i2c_read_single(CLOCK_CHIP, 0x04);
    while(l >> 7 == 1)
    {
        __delay_us(100);
        i2c_write_single(0, CLOCK_CHIP, 0x04);
        l = i2c_read_single(CLOCK_CHIP, 0x04);
    }
    
    
}


BOOL set_time(TIMEDAT *set_time)
{
    //create buffer
    char time_buffer[6];
    
    //Second
    time_buffer[0] = VAL_TO_BCD(set_time->second);
    time_buffer[1] = VAL_TO_BCD(set_time->minute);
    time_buffer[2] = VAL_TO_BCD(set_time->hour);
    time_buffer[3] = VAL_TO_BCD(set_time->day);
    time_buffer[4] = VAL_TO_BCD(set_time->month);
    time_buffer[5] = VAL_TO_BCD(set_time->year);
    
    i2c_write(time_buffer, 6, CLOCK_CHIP, 0x4);
    
    time_quality = TIM_QLTY_RST;
    
    return TRUE;
}


TIMEDAT *get_time(void)
{
    char data[7];
    
    if(time_quality > 0)
        time_quality--;

    if(i2c_read(CLOCK_CHIP, 0x4, 7, data) != COMM_ERR)
    {
        curr_time.second = BCD_TO_VAL(data[0]);
        curr_time.minute = BCD_TO_VAL(data[1]);
        curr_time.hour = BCD_TO_VAL(data[2]);
        curr_time.day = BCD_TO_VAL(data[3]);
        curr_time.month = BCD_TO_VAL(data[5]);
        curr_time.year = BCD_TO_VAL(data[6]);
    }
    
    MIN_INT = FALSE;
    
    return &curr_time;
}


BOOL time_needs_to_be_set(void)
{
    return time_quality <= 0;
}