#include "audio.h"
#include <xc.h>
#include "bluetooth.h"
#include "io.h"
#include "pwm.h"
#include "adc.h"

BOOL aud_jack_stat;

void init_audio(void)
{
    aud_jack_stat = PORTAbits.RA5;
}

void audio_jack_process()
{
    aud_jack_stat = PORTAbits.RA5;
    if(aud_jack_stat)
    {
        //Stop audio mode
        //enter clock mode
        //stop_adc(); //Stop ADC conversion
        set_status_led(CLOCKMD, FALSE); //set status led
    }
    else /*if(get_bt_connected())*/
    {
        //BT is connected and audio is playing. Start Audio mode
        set_status_led(AUDMD, FALSE);
    }
    //Otherwise just ignore
    
    AUD_JACK_INT = FALSE;
}