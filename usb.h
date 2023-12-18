/* 
 * File:   usb.h
 * Author: heinrich
 *
 * Created on June 17, 2018, 5:13 PM
 */
#ifndef USB_H
#define	USB_H

void usb_process(void);
BOOL usb_str_is_STATUS(void);
void send_OK(void);
void send_Unknown_Command(void);

#endif	/* USB_H */

