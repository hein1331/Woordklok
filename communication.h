#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#include <xc.h>
#include "stdtypes.h"

//Errors
#define COMM_ERR   -1

//Limits
#define MAX_STR_LEN     200

//I2C Slave operations
#define WRITE(addr)      addr &= ~(1 << 0)
#define READ(addr)       addr |= 1 << 0


//I2C Slave Addresses
#define CLOCK_CHIP      0b10100010
#define LED_CONT(board) (char)((board == 0)*0b10111110 + \
                        (board == 1)*0b10100000 + \
                        (board == 2)*0b10100110)

//I2C Serial interrupt status
#define USB_REC_INT      PIR3bits.RC2IF
#define BT_REC_INT       PIR3bits.RC1IF


//Init Functions
void init_comm(void);


//USB Functions
BOOL write_usb_serial(char data[]);
void process_usb_serial(void);
BOOL usb_msg_received(void);
void transfer_usb(void);
char* get_usb_serial_str(void);


//Bluetooth Functionns
BOOL write_bt_serial(char* data);
void process_bt_serial(void);
BOOL bt_msg_received(void);
void transfer_bt(void);
char* get_bt_serial_str(void);

//I2C Functions
BOOL i2c_write_single(char data, SLAVE_ADDR addr, REG_ADDR reg_addr);
BOOL i2c_write(char *data, int data_len,  SLAVE_ADDR addr, REG_ADDR reg_addr);
char i2c_read_single(SLAVE_ADDR addr, REG_ADDR reg_addr);
BOOL i2c_read(SLAVE_ADDR addr, REG_ADDR reg_addr, int data_read_len, char* data);
void I2C_Master_Wait(void);
void I2C_Master_Start(void);
void I2C_Master_Write(unsigned d);
unsigned short I2C_Master_Read(BOOL final_read);
void I2C_Master_Stop();

#endif	/* COMMUNICATION_H */

