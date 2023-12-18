#ifndef ADC_H
#define	ADC_H

#include "stdtypes.h"

/* General Idea
 * Update LED rate: LED_REF_RAT
 * 16 Buckets at 537Hz range
 * 32 ADC Samples
 * ADC sample time: 1/(LED_REF_RAT*32)
 * 
 * 
 * 
 */

#define PRESCALAR_ADC       8.0f

#define ADC_INT             PIR1bits.ADIF

float ADC_SAM_FREQ;
unsigned int ADC_TIM_VAL;

//Functions
void init_adc(void);
void start_adc(void);
void stop_adc(void);
void process_adc(void);
BOOL isFFTReady(void);
void processFFT(void);

#endif	/* ADC_H */

