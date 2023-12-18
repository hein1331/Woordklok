#include <string.h>
#include <pic18f27k40.h>
#include <xc.h>

#include "stdtypes.h"
#include "usb.h"
#include "communication.h"

char data_usb[MAX_STR_CMD]; //Used to compare strings
char *tim_dat; // Saved time string
char *usb_str;



void usb_process(void)
{
    usb_str = get_usb_serial_str();
    if(usb_str_is_STATUS())
    {
        //Send ok
        send_OK();
    }
    else
    {
        //Unknown command
        send_Unknown_Command();
    }
}

BOOL usb_str_is_STATUS(void)
{
    data_usb[0] = 'S';
    data_usb[1] = 'T';
    data_usb[2] = 'A';
    data_usb[3] = 'T';
    data_usb[4] = 'U';
    data_usb[5] = 'S';
    return (strncmp(usb_str, data_usb, 6) == 0);
}


void send_OK(void)
{
    data_usb[0] = 'O';
    data_usb[1] = 'K';
    data_usb[2] = '\n';
    write_usb_serial(data_usb);
}


void send_Unknown_Command(void)
{
    data_usb[0] = 'U';
    data_usb[1] = 'n';
    data_usb[2] = 'k';
    data_usb[3] = 'n';
    data_usb[4] = 'o';
    data_usb[5] = 'w';
    data_usb[6] = 'n';
    data_usb[7] = ' ';
    data_usb[8] = 'C';
    data_usb[9] = 'o';
    data_usb[10] = 'm';
    data_usb[11] = 'm';
    data_usb[12] = 'a';
    data_usb[13] = 'n';
    data_usb[14] = 'd';
    data_usb[15] = '\n';
    write_usb_serial(data_usb);
}