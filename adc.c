#include <pic18f27k40.h>

#include "adc.h"

// Globals
short imaginaryNumbers[64];
short realNumbers[64];
short i;
BOOL fft_ready;


void init_adc(void)
{
    ADC_TIM_VAL = MAX16BIT - (unsigned int) (FREQ/4.f/PRESCALAR_ADC/ADC_SAM_FREQ);
    ADC_SAM_FREQ = LED_REF_RAT*32.0f;
    
    //Init timer 1 at ADC Sampling Frequency
    T1CONbits.ON = DISABLED; //Turn off timer 1
    T1CONbits.RD16 = ENABLED; //Single 16 bit operations
    TMR1CLK = 0b0001; //Set the clock source to HFINTOSC/4
    TMR1 = ADC_TIM_VAL;
    
    //Disable ADC
    ADCON0bits.ADON = DISABLED;
    
    //Non Continuos ADC
    ADCON0bits.ADCONT = DISABLED;
    
    //Supply clock from FOSC
    ADCON0bits.ADCONT = 0;
    ADCLKbits.ADCS = 0;
    
    //Select channel at ANA1
    ADPCH = 000001;
    
    //Set positive voltage reference to Vcc
    ADREFbits.ADPREF = 0;
    
    //Set negative voltage reference to Vss
    ADREFbits.ADNREF = 0;
    
    //Set the conversion read method to right adjusted
    ADCON0bits.ADFM = 1;
    
    //Set autotrigger
    ADACT = 0b00011;   

    //Enable ADC
    ADCON0bits.ADON = ENABLED;
    
    i = 0;
}


void start_adc(void)
{
    T1CONbits.ON = ENABLED;
}


void stop_adc(void)
{
    T1CONbits.ON = DISABLED;
}

void process_adc(void)
{
    //Use value for calculation
    realNumbers[i++] = ((unsigned short)(ADRESH << 8) + (unsigned short)ADRESL) - 512;
    if(i >= 64)
    {
        stop_adc();
        fft_ready = TRUE;
    }
    ADC_INT = FALSE;
}


BOOL isFFTReady(void)
{
    return fft_ready;
}


void processFFT(void)
{
    //Process FFT
    
    //Reset ADC save
    fft_ready = FALSE;
    start_adc();
    
}