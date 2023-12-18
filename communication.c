#include <xc.h>
#include <pic18f27k40.h>

#include "stdtypes.h"
#include "communication.h"
#include "interrupt.h"

//testing
#include "bluetooth.h"
#include "pwm.h"

//In progress values
volatile BOOL i2c_bus_busy; //Represents I2C operation is in progress

//Variables used by serial comm
char bt_ser_str[MAX_STR_LEN]; //Bluetooth serial string
char usb_ser_str[MAX_STR_LEN]; //USB serial string

char bt_ser_str_dat[MAX_STR_LEN];
char usb_ser_str_dat[MAX_STR_LEN];

unsigned int bt_ser_str_cntr; //Bluetooth serial string counter
unsigned int usb_ser_str_cntr; //USB serial string counter

unsigned int bt_ser_str_read_start;
unsigned int usb_ser_str_read_start;

unsigned int bt_str_ready; //Bluetooth string ready boolean
unsigned int usb_str_ready; //USB string ready boolean


/******************************************************************************/
/*********************************Init Function********************************/
/******************************************************************************/

//Init MCU communication
void init_comm(void)
{
    //Init i2c. TODO: Split the register into into individual bits
    SSP1CON1 = 0b00101000; 
    SSP1CON2 = 0;
    SSP1ADD = 159;
    SSP1STAT = 0;     
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    
    i2c_bus_busy = FALSE; //Ensure bus is not busy
    
    /*
    //Init Bluetooth Serial
    SP1BRG = 102; //Set baud rate. 9600 @ 64mHz
    TX1STAbits.TX9 = DISABLED; //Disable 9 bit data transmit
    TX1STAbits.TXEN = ENABLED; //Enable transmit
    TX1STAbits.SYNC = DISABLED; //Async Mode
    TX1STAbits.SENDB = DISABLED; //Disable break char
    BAUD1CONbits.BRG16 = FALSE; 
    TX1STAbits.BRGH = FALSE; //Set to low Baud Rate
    RC1STAbits.SPEN = ENABLED; //Enable serial port
    RC1STAbits.RX9 = DISABLED; //Disable 9 bit receive
    RC1STAbits.CREN = ENABLED; //Enable receive

    bt_ser_str_cntr = 0; //Reset bt string position counter
    bt_ser_str_read_start = 0;
    bt_ser_str[0] = 0; //Reset bt string
    bt_str_ready = 0; //Reset bt string false
    
    
    //Init USB serial
    SP2BRG = 102; //Set baud rate. 9600 @ 64mHz
    TX2STAbits.TX9 = DISABLED; //Disable 9 bit data transmit
     TX2STAbits.TXEN = ENABLED; //Enable transmit
   TX2STAbits.SYNC = DISABLED; //Async Mode
    TX2STAbits.SENDB = DISABLED; //Disable break char
    BAUD2CONbits.BRG16 = FALSE; 
    TX2STAbits.BRGH = FALSE; //Set to low Baud Rate
    RC2STAbits.SPEN = ENABLED; //Enable serial port
    RC2STAbits.RX9 = DISABLED; //Disable 9 bit receive
    RC2STAbits.CREN = ENABLED; //Enable receive

    usb_ser_str_cntr = 0; //Reset USB string position counter
    usb_ser_str_read_start = 0;
    usb_ser_str[0] = 0; //Reset USB string
    usb_str_ready = FALSE; //Reset USB string false*/
}


/******************************************************************************/
/*********************************I2C Functions********************************/
/******************************************************************************/

//Write a single byte to slave's register
BOOL i2c_write_single(char data, SLAVE_ADDR addr, REG_ADDR reg_addr)
{
    return i2c_write(&data, 1, addr, reg_addr);
}

//Write an array of bytes to a slave's incremental registers
BOOL i2c_write(char *data, int data_len,  SLAVE_ADDR addr, REG_ADDR reg_addr)
/*Write a single byte to a slave at the specified register*/
{
    //Do not allow write when I2C bus is active
    if(i2c_bus_busy)
    {
        set_status_led(ERROR, TRUE);
        return COMM_ERR;
    }
    
    if(SSP1CON1bits.WCOL)
    {
        set_status_led(ERROR, TRUE);
        return COMM_ERR;
    }
        
    i2c_bus_busy = TRUE;
    set_status_led(CLOCKMD, FALSE);
    
    //Start condition
    I2C_Master_Start();     
    //7 bit address + Write    
    I2C_Master_Write(WRITE(addr));  
    //Write register address to write to
    I2C_Master_Write(reg_addr);
    for(int index = 0; index < data_len; index++)
    {
        //Write data
        I2C_Master_Write(data[index]);    
    }
    
    
    I2C_Master_Stop();          //Stop condition
    
    //Free bus
    i2c_bus_busy = FALSE;
    return TRUE;
}

//Read a single byte from a slave's register
char i2c_read_single(SLAVE_ADDR addr, REG_ADDR reg_addr)
{
    char temp;
    i2c_read(addr, reg_addr, 1, &temp);
    return temp;
}

//Read an array of bytes from a slave's inncrementalregisters
BOOL i2c_read(SLAVE_ADDR addr, REG_ADDR reg_addr, int data_read_len, char* data)
/*Read a register address of a slave*/
{  
    //Do not allow write when I2C bus is active
    if(i2c_bus_busy)
        return COMM_ERR;
    i2c_bus_busy = TRUE;
    
    //Start condition
    I2C_Master_Start(); 
    //7 bit address + Write
    I2C_Master_Write(WRITE(addr));     
    //Write the register address to read
    I2C_Master_Write(reg_addr);
    //Stop I2C
    I2C_Master_Stop();
    
    //Wait for bus to clear
    __delay_us(30);
    
    //Start condition
    I2C_Master_Start(); 
    
    //Write the address of slave and read bit
    I2C_Master_Write(READ(addr));
    
    //WAit for I2C bus
    I2C_Master_Wait();
    
    //Read data and save to array
    for(int index = 0; index < data_read_len - 1; index++)
    {
        data[index] = I2C_Master_Read(FALSE);
    }
    //Read final bit and acknowledge
    data[data_read_len - 1] = I2C_Master_Read(TRUE);
    //Send stop bit
    I2C_Master_Stop();          //Stop condition
    
    //Free bus
    i2c_bus_busy = FALSE;
    return TRUE;
}


//Wait until I2C is ready
void I2C_Master_Wait(void)
{
  while ((SSP1STATbits.R_W) || (SSP1CON2 & 0x1F)); //Transmit is in progress
}

//Send start bit
void I2C_Master_Start(void)
{
  I2C_Master_Wait();   
  //Initiate start condition
  SSP1CON2bits.SEN = 1;             
}

//Write byte to I2C bus
void I2C_Master_Write(unsigned d)
{
  I2C_Master_Wait();
  //Write data to SSPBUF
  SSP1BUF = d;         
}

//Read byte from I2C bus
unsigned short I2C_Master_Read(BOOL final_read)
{
  unsigned short temp;
  I2C_Master_Wait();
  SSP1CON2bits.RCEN = 1;
  PIR3bits.SSP1IF = 0;
  I2C_Master_Wait();
  while(!PIR3bits.SSP1IF);
  //Read data from SSPBUF
  temp = SSP1BUF;      
  PIR3bits.SSP1IF = 0;
  I2C_Master_Wait();
  //Acknowledge bit
  SSP1CON2bits.ACKDT = (unsigned) final_read;  
  //Acknowledge sequence  
  SSP1CON2bits.ACKEN = 1;          
  return temp;
}

//Send stop bit
void I2C_Master_Stop()
{
  I2C_Master_Wait();
  SSP1CON2bits.PEN = 1;
}


/******************************************************************************/
/**********************Bluetooth Communication*********************************/
/******************************************************************************/

//Write a \n terminated string to the BT serial bus
BOOL write_bt_serial(char* data)
{
    if(!TX1STAbits.TRMT)
                return FALSE;
    int i = 0;
    //TX1REG = '\r';
    while(data[i] != '\n' && data[i] != '\r')
    {           
        while(!TX1STAbits.TRMT);
        TX1REG = data[i++];
        if (i >= MAX_STR_LEN)
            break;
    }
    TX1REG = '\r';
    return TRUE;
}

//Process a byte received from bluetooth serial
void process_bt_serial(void)
{
    if(!bt_msg_received())
    {
        
    }
            
    char data = RC1REG;
    if(RC1STAbits.OERR)
    {
        RC1STAbits.CREN = 0;
        RC1STAbits.CREN = ENABLED; //Enable receive
    }
    bt_ser_str[bt_ser_str_cntr++] = data;
    if(data == '\r')
    {
        bt_str_ready++;
    }
    
    if(bt_ser_str_cntr >= MAX_STR_LEN )
    {
        bt_ser_str_cntr = 0;
    }
}

//Returns if bluetooth string is ready
BOOL bt_msg_received(void)
{
    return bt_str_ready > 0;
}

//Transfer a received bluetooth string to USB
void transfer_bt(void)
{
    if(bt_msg_received())
    {
        write_usb_serial(get_bt_serial_str());
    }
}

//Returns the received USB string and resets USB string ready boolean
char* get_bt_serial_str(void)
{
    if(!bt_msg_received())
        return 0;
    
    char dat;
    unsigned int i = 0;
    
    do
    {
        dat = bt_ser_str[bt_ser_str_read_start++];
        bt_ser_str_dat[i++] = dat;
        
        if(bt_ser_str_read_start >= MAX_STR_LEN)
            bt_ser_str_read_start = 0;
    } while(dat != '\r');
    
    bt_str_ready--;
    
    return bt_ser_str_dat;
}

/******************************************************************************/
/****************************USB Communication*********************************/
/******************************************************************************/

//Write a \n terminated string to the USB serial bus
BOOL write_usb_serial(char *data)
{
    if(!TX2STAbits.TRMT)
        return FALSE;
    int i = 0;
    while(data[i] != '\n' && data[i] != '\r')
    {           
        while(!TX2STAbits.TRMT);
        TX2REG = data[i++];
        if (i >= MAX_STR_LEN)
            break;
    }
    TX2REG = '\r';
    return TRUE;
}

//Process a byte received from USB serial
void process_usb_serial(void)
{
    char data = RC2REG;
    if(RC2STAbits.OERR)
    {
        RC2STAbits.CREN = 0;
        RC2STAbits.CREN = ENABLED; //Enable receive
    }
    usb_ser_str[usb_ser_str_cntr++] = data;
    if(data == '\n' || data == '\r')
    {
        usb_str_ready++;
    }
    
    if(usb_ser_str_cntr >= MAX_STR_LEN )
    {
        usb_ser_str_cntr = 0;
    }
}


//Returns if bluetooth string is ready
BOOL usb_msg_received(void)
{
    return usb_str_ready > 0;
}

//Transfer a received bluetooth string to USB
void transfer_usb(void)
{
    if(usb_msg_received())
    { 
        write_bt_serial(get_usb_serial_str());
    }
}

//Returns the received USB string and resets USB string ready boolean
char* get_usb_serial_str(void)
{
    if(!usb_msg_received())
        return 0;
    
    char dat;
    unsigned int i = 0;
    
    do
    {
        dat = usb_ser_str[usb_ser_str_read_start++];
        usb_ser_str_dat[i++] = dat;
        
        if(usb_ser_str_read_start >= MAX_STR_LEN)
            usb_ser_str_read_start = 0;
    } while(dat != '\n' && dat != '\r');
    
    usb_str_ready--;
    
    return usb_ser_str_dat;
}
