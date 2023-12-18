#include <string.h>
#include <pic18f27k40.h>
#include <xc.h>

#include "bluetooth.h"
#include "communication.h"
#include "pwm.h"
#include "clock.h"
#include "stdtypes.h"
#include "adc.h"

//Bluetooth strings
char data[MAX_STR_LEN]; //Used to compare strings
char *tim_dat; // Saved time string
char *bt_str;

//Bluetooth connection
BOOL bt_connected; //Connected bluetooth

/******************************************************************************/
/*********************************Init Function********************************/
/******************************************************************************/
//Init bluetooth
void init_bt(void)
{
    //Set BT LED as discoverable
    set_bt_led(ENABLED, FALSE);
    
    //Set BT as not connected
    bt_connected = FALSE;
}


/******************************************************************************/
/*********************************Get Functions********************************/
/******************************************************************************/
BOOL get_bt_connected(void)
{
    return bt_connected;
}


/******************************************************************************/
/********************************Send Functions********************************/
/******************************************************************************/
//Send a request for status to bluetooth. TODO: Write response recieve event
void send_status(void)
{
    data[0] = 'S';
    data[1] = 'T';
    data[2] = 'A';
    data[3] = 'T';
    data[4] = 'U';
    data[5] = 'S';
    data[6] = '\n';
    //write_bt_serial(data);
    write_usb_serial(data);
    
}

//Send a request to play a tone. TODO: Test and decide what to play tones for
void send_tone(void)
{
    data[0] = 'T';
    data[1] = 'O';
    data[2] = 'N';
    data[3] = 'E';
    data[4] = ' ';
    data[5] = 'V';
    data[6] = ' ';
    data[7] = '1';
    data[8] = '2';
    data[9] = '8';
    data[10] = ' ';
    data[11] = 'T';
    data[12] = 'I';
    data[13] = ' ';
    data[14] = '0';
    data[15] = ' ';
    data[16] = 'N';
    data[17] = ' ';
    data[18] = 'C';
    data[19] = '4';
    data[20] = ' ';
    data[21] = 'L';
    data[22] = ' ';
    data[23] = '8';
    data[24] = ' ';
    data[25] = 'N';
    data[26] = ' ';
    data[27] = 'D';
    data[28] = '4';
    data[29] = ' ';
    data[30] = 'L';
    data[31] = ' ';
    data[32] = '8';
    data[33] = ' ';
    data[34] = 'N';
    data[35] = ' ';
    data[36] = 'E';
    data[37] = '4';
    data[38] = ' ';
    data[39] = 'L';
    data[40] = ' ';
    data[41] = '8';
    data[42] = ' ';
    data[43] = 'N';
    data[44] = ' ';
    data[45] = 'F';
    data[46] = '4';
    data[47] = ' ';
    data[48] = 'L';
    data[49] = ' ';
    data[50] = '8';
    data[51] = ' ';
    data[52] = 'N';
    data[53] = ' ';
    data[54] = 'G';
    data[55] = '4';
    data[56] = ' ';
    data[57] = 'L';
    data[58] = ' ';
    data[59] = '8';
    data[60] = ' ';
    data[61] = 'N';
    data[62] = ' ';
    data[63] = 'A';
    data[64] = '4';
    data[65] = ' ';
    data[66] = 'L';
    data[67] = ' ';
    data[68] = '8';
    data[69] = ' ';
    data[70] = 'N';
    data[71] = ' ';
    data[72] = 'B';
    data[73] = '4';
    data[74] = ' ';
    data[75] = 'L';
    data[76] = ' ';
    data[77] = '8';
    data[78] = ' ';
    data[79] = 'N';
    data[80] = ' ';
    data[81] = 'C';
    data[82] = '5';
    data[83] = ' ';
    data[84] = 'L';
    data[85] = ' ';
    data[86] = '8';
    data[87] = '\n';
    
    write_bt_serial(data);
}


/******************************************************************************/
/***************************BT Event Processing********************************/
/******************************************************************************/
//Process a received bluetooth strig
void bt_process(void)
{
    bt_str = get_bt_serial_str();
    if(bt_str_is_PAIR_OK())
    {
        //Set bluetooth led to connected
        set_bt_led(ENABLED, TRUE);
        
        //Set BT as connected
        bt_connected = TRUE;
    }
    else if(bt_str_is_CLOSE_OK() || bt_str_is_LINK_LOSS())
    {
        //Set BT LED as discoverable
        set_bt_led(ENABLED, FALSE);
        
        //Set BT as disconnected
        bt_connected = FALSE;
        
        //Stop Music
        //stop_adc(); //Stop ADC conversion
        send_Music_Paused();
        set_status_led(CLOCKMD, TRUE); //set status led
    }
    else if(bt_str_is_RECV_SPP())
    {
        //Process time/other data from mobile
        //Format: yyyymmddhhmmss
        TIMEDAT time;
        tim_dat = bt_str;
        time.year = (unsigned) str_to_int(&tim_dat[9], 4);
        time.month = (unsigned) str_to_int(&tim_dat[13], 2);
        time.day = (unsigned) str_to_int(&tim_dat[15], 2);
        time.hour = (unsigned) str_to_int(&tim_dat[17], 2);
        time.minute = (unsigned) str_to_int(&tim_dat[19], 2);
        time.second = (unsigned) str_to_int(&tim_dat[21], 2);
        set_time(&time);
    }
    else if(bt_str_is_PAIR_ERROR())
    {
        //Set error led
        set_status_led(ERROR, FALSE);
        
        //Set BT as disconnected
        bt_connected = FALSE;
    }
    else if(bt_str_is_AVRCP_PLAY())
    {
        //enter play mode, 
        //start_adc();//Start ADC conversion
        send_Music_Playing();
        set_status_led(AUDMD, TRUE); //set status led
    }

    else if(bt_str_is_AVRCP_PAUSE() || bt_str_is_AVRCP_STOP())
    {
        //enter clock mode
        //stop_adc(); //Stop ADC conversion
        send_Music_Paused();
        set_status_led(CLOCKMD, TRUE); //set status led
    }
    else
    {
        //write_usb_serial(bt_str);
    }
}

int str_to_int(char* str_to_convert, int chars_to_convert)
{
    int val = 0;
    for(int i = 0; i < chars_to_convert; i++)
    {
        val += ((int)str_to_convert[i] - 48) * (10^i);
    }
    return val;
}


BOOL bt_str_is_AVRCP_PLAY(void)
{
    data[0] = 'A';
    data[1] = 'V';
    data[2] = 'R';
    data[3] = 'C';
    data[4] = 'P';
    data[5] = '_';
    data[6] = 'P';
    data[7] = 'L';
    data[8] = 'A';
    data[9] = 'Y';
    return (strncmp(bt_str, data, 10) == 0);
}


BOOL bt_str_is_PAIR_OK(void)
{
    data[0] = 'P';
    data[1] = 'A';
    data[2] = 'I';
    data[3] = 'R';
    data[4] = '_';
    data[5] = 'O';
    data[6] = 'K';
    return (strncmp(bt_str, data, 7) == 0);
}


BOOL bt_str_is_LINK_LOSS(void)
{
    data[0] = 'L';
    data[1] = 'I';
    data[2] = 'N';
    data[3] = 'K';
    data[4] = '_';
    data[5] = 'L';
    data[6] = 'O';
    data[7] = 'S';
    data[8] = 'S';
    return (strncmp(bt_str, data, 9) == 0);
}


BOOL bt_str_is_CLOSE_OK(void)
{
    data[0] = 'C';
    data[1] = 'L';
    data[2] = 'O';
    data[3] = 'S';
    data[4] = 'E';
    data[5] = '_';
    data[6] = 'O';
    data[7] = 'K';
    return (strncmp(bt_str, data, 8) == 0);
}


BOOL bt_str_is_RECV_SPP(void)
{
    data[0] = 'R';
    data[1] = 'E';
    data[2] = 'C';
    data[3] = 'V';
    data[4] = ' ';
    data[5] = 'S';
    data[6] = 'P';
    data[7] = 'P';
    return (strncmp(bt_str, data, 8) == 0);
}


BOOL bt_str_is_PAIR_ERROR(void)
{
    data[0] = 'P';
    data[1] = 'A';
    data[2] = 'I';
    data[3] = 'R';
    data[4] = '_';
    data[5] = 'E';
    data[6] = 'R';
    data[7] = 'R';
    data[8] = 'O';
    data[9] = 'R';
    return (strncmp(bt_str, data, 10) == 0);
}


BOOL bt_str_is_AVRCP_STOP(void)
{
    data[0] = 'A';
    data[1] = 'V';
    data[2] = 'R';
    data[3] = 'C';
    data[4] = 'P';
    data[5] = '_';
    data[6] = 'S';
    data[7] = 'T';
    data[8] = 'O';
    data[9] = 'P';
    return (strncmp(bt_str, data, 10) == 0);
}


BOOL bt_str_is_AVRCP_PAUSE(void)
{
    data[0] = 'A';
    data[1] = 'V';
    data[2] = 'R';
    data[3] = 'C';
    data[4] = 'P';
    data[5] = '_';
    data[6] = 'P';
    data[7] = 'A';
    data[8] = 'U';
    data[9] = 'S';
    data[10] = 'E';
    return (strncmp(bt_str, data, 11) == 0);
}


void send_Music_Playing(void)
{
    data[0] = 'M';
    data[1] = 'u';
    data[2] = 's';
    data[3] = 'i';
    data[4] = 'c';
    data[5] = ' ';
    data[6] = 'P';
    data[7] = 'l';
    data[8] = 'a';
    data[9] = 'y';
    data[10] = 'i';
    data[11] = 'n';
    data[12] = 'g';
    data[13] = '\n';
    
    write_usb_serial(data);
}

void send_Music_Paused(void)
{
    data[0] = 'M';
    data[1] = 'u';
    data[2] = 's';
    data[3] = 'i';
    data[4] = 'c';
    data[5] = ' ';
    data[6] = 'P';
    data[7] = 'a';
    data[8] = 'u';
    data[9] = 's';
    data[10] = 'e';
    data[11] = 'd';
    data[12] = '\0';
    
    write_usb_serial(data);
}