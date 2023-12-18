/*
 * File:   main.c
 * Author: heinrich
 *
 * Created on September 17, 2017, 1:02 PM
 */

#include "configuration.h"
#include <xc.h>
#include "stdtypes.h"
#include "io.h"
#include "pwm.h"
#include "communication.h"
#include "adc.h"
#include "clock.h"
#include "led.h"
#include "bluetooth.h"
#include "audio.h"
#include "interrupts.h"
#include "usb.h"

TIMEDAT *time;


#define _XTAL_FREQ 64000000

void main(void) 
{
    PCON0;
    //Init all functions
    init_io();    
    //Init PWM
    init_pwm();   
    //Init communication
    init_comm();
    //Init ADC
    //init_adc();  
    //Init Audio
    //init_audio();
    
    
    //Wait for I2C to stabilize
    //__delay_ms(200);
    //Init clock
    //init_clock();
    //Init LED Controller
    init_led();
    //Init Bluetooth
    //init_bt();

    //Init and start interrupts
    //init_interrupt();
    //set_status_led(INIT, TRUE);   
    
    __delay_ms(1000);
    
    //Start in clock mode
    //set_status_led(CLOCKMD, TRUE);
    
    //Init program switch
    //prog_sw_process();

    //open_short_test(0);
    char all_on[192];
    for(unsigned int i = 0; i < 192; i++)
            all_on[i] = 0xFF;
    __delay_ms(10);
    set_page(0, LED_CONT(2));
    __delay_ms(10);
    char m = i2c_read_single(LED_CONT(2), 0x11);
    while(TRUE)
    {
        for(int board = 2; board < 3; board++)
        {
            set_page(3, LED_CONT(board));
            __delay_ms(10);
            i2c_write_single(255, LED_CONT(board), 0x01);
            __delay_ms(10);
            set_page(0, LED_CONT(board));
            __delay_ms(10);
            i2c_write(all_on, 0xBF,  LED_CONT(board), 0x00);
            __delay_ms(10);
            set_page(3, LED_CONT(board));
            __delay_ms(10);
            i2c_write_single(0b00000101, LED_CONT(board), 0x00);
            __delay_ms(10);
            set_page(1, LED_CONT(board));
            __delay_ms(10);
            i2c_write_single(0xFF, LED_CONT(board), 0x00);
            __delay_ms(10);
            i2c_write_single(0xFF, LED_CONT(board), 0x01);
            __delay_ms(10);
            i2c_write_single(0xFF, LED_CONT(board), 0x02);
            __delay_ms(10);
            set_page(0, LED_CONT(board));
            __delay_ms(10);
            m = i2c_read_single(LED_CONT(board), 0x18);
            __delay_ms(10);
            char n = i2c_read_single(LED_CONT(board), 0x30);
            __delay_ms(10);
            set_page(3, LED_CONT(board));
            __delay_ms(10);
            i2c_write_single(1, LED_CONT(board), 0x00);
            //i2c_write(all_on, 192, LED_CONT(board), 0x0);
            __delay_ms(10);
            __delay_ms(500);
            set_status_led(INIT, FALSE);
            __delay_ms(500);
            //set_status_led(CLOCKMD, FALSE);
            
        }
       /* //Poll variables for changes
        if(isFFTReady())
            processFFT();
        //Poll time quality
        if(time_needs_to_be_set())
            set_status_led(TIMSET, FALSE);*/
        //if(bt_msg_received())
            //bt_process();
        //if(usb_msg_received())
            //usb_process();
        
        /*set_status_led(BADTQ, TRUE);
        __delay_ms(1000);
        set_status_led(ERROR, TRUE);
        __delay_ms(1000);
        set_status_led(TIMSET, TRUE);
        __delay_ms(1000);*/
    }
}


void interrupt high_pri_isr(void)
/*ISR Function to handle all the high priority interrupts*/
{
    //USB Serial Received
    if(USB_REC_INT)  
        process_usb_serial();
    
    //Bluetooth Serial Received
    if(BT_REC_INT)
        process_bt_serial();
    
    //ADC Conversion Complete
    if(ADC_INT)
        process_adc();
    
    //PWM Interrupt
    if(PWM_INT)
        pwm_process();
}


void interrupt low_priority low_pri_isr(void)
/*ISR Function to handle all the low priority interrupts*/
{
    //Minute interrupt
    if(/*!time_needs_to_be_set() && */MIN_INT)
        time = get_time();

    //LED Update Interrupt
    if(SCR_UPD_INT)
        screen_update(time);
    
    //Audio Jack Status Interrupt
    if(AUD_JACK_INT)
        audio_jack_process();
    
    //Programming Switch Interrupt
    if(PROG_SWIT_INT)
        prog_sw_process();
}