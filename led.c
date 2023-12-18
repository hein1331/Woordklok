#include <stdlib.h>
#include "led.h"
#include "communication.h"
#include "clock.h"

char led_cont[3][LED_CNT_CONT];
int counter;
char dat[47];

/*
 * 12|B B B B B B B B B B  B  B  B  B  B  B
 * 11|G G G G G G G G G G  G  G  G  G  G  G
 * 10|R R R R R R R R R R  R  R  R  R  R  R
 * 9 |B B B B B B B B B B  B  B  B  B  B  B
 * 8 |G G G G G G G G G G  G  G  G  G  G  G
 * 7 |R R R R R R R R R R  R  R  R  R  R  R
 * 6 |B B B B B B B B B B  B  B  B  B  B  B
 * 5 |G G G G G G G G G G  G  G  G  G  G  G
 * 4 |R R R R R R R R R R  R  R  R  R  R  R
 * 3 |B B B B B B B B B B  B  B  B  B  B  B
 * 2 |G G G G G G G G G G  G  G  G  G  G  G
 * 1 |R R R R R R R R R R  R  R  R  R  R  R
 *    1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
 *    
 */

//Initalize the LED controllers
void init_led(void)
{
    LED_TIM_VAL = MAX16BIT - (unsigned int) (FREQ/4.f/PRESCALAR_LED/LED_REF_RAT);
    
    //Init timer 2 at LED Refresh Rate
    /*T2CONbits.ON = DISABLED; //Turn off timer 2
    T2CONbits.CKPS = 0b111;//Prescalar of 128 
    T2CLKCON = 0b0001; //Set the clock source to HFINTOSC/4
    TMR2 = LED_TIM_VAL;
    //T2CONbits.ON = ENABLED; 
    
    counter = 0;
    clear_led();*/
    
    for(int board = 2; board < BOARD_CNT; board++)
    {
        //Set SW Reset for LED Chip Off
        set_page(3, LED_CONT(board));
        __delay_ms(1);
        i2c_write_single(0b00000001, LED_CONT(board), 0x00);
        __delay_ms(1);
        //Set current to 30mA
        //set_led_current(30, board);
        set_page(3, LED_CONT(board));
        i2c_write_single(0xF0, LED_CONT(board), 0x01);
        __delay_ms(1);
        //Turn on all LEDs
        set_page(0, LED_CONT(board));
        char all_on[24];
        for(unsigned int i = 0; i < 24; i++)
            all_on[i] = 0xFF;
        __delay_ms(1);
        i2c_write(all_on, 24, LED_CONT(board), 0x0);
    }
    
    //refresh_led;
    
}

void open_short_test(int board)
{
    char all_led[24];
    for(unsigned int i = 0; i < 24; i++)
        all_led[i] = 0x00;
    
    set_page(0, LED_CONT(board));
    i2c_write(all_led, 24, LED_CONT(board), 0x0);
    
    set_page(3, LED_CONT(board));
    i2c_write_single(0x01, LED_CONT(board), 0x01);
    i2c_write_single(0b01000101, LED_CONT(board), 0x00);
    
    __delay_ms(5);
    set_page(0, LED_CONT(board));
    
    i2c_read(LED_CONT(board), 0x18, 47, dat);
    
    int l = 2;
    
}

//Clear the screen
void clear_led(void)
{
    for(unsigned int i = 0; i < BOARD_CNT; i++)
        for(unsigned int j = 0; j < LED_CNT_CONT; j++)
            led_cont[i][j] = 0;
    refresh_led();
}

void set_led_current(unsigned int curr_ma, int board)
{
    //Set LED current to 30mA: GCC = (Iout * Rext * 256)/840
    unsigned int gcc = (curr_ma * REXT_KOHM * 256)/840;
    set_page(3, LED_CONT(board));
    i2c_write_single(gcc, LED_CONT(board), 0x01);
}

//Update all the leds on the board
BOOL refresh_led(void)
{
    BOOL succ = TRUE;
    for(unsigned int board = 0; board < BOARD_CNT; board++)
    {
        succ &= set_page(1, LED_CONT(board));
        succ &= i2c_write(led_cont[board], LED_CNT_CONT, LED_CONT(board), 0x0);
    }
    return succ;
}

//Set an individual pixel on the screen
void set_pixel_pwm(unsigned int x, unsigned int y, char pwm, unsigned int clr)
{
    if(x > 16 || y > 12)
        return;
    unsigned int index = (unsigned) (x + (y-1)*36 + clr*12);
    unsigned int board = index/LED_CNT_CONT;
    index -= board*LED_CNT_CONT;
    
    //led_cont[board][index] = pwm;
    
    led_cont[0][x] = pwm;
    led_cont[1][x] = pwm;
    led_cont[2][x] = pwm;
}

void set_pixel(unsigned int x, unsigned int y, RGBLED *clr)
{
    set_pixel_pwm(x,y,clr->R, R);
    set_pixel_pwm(x,y,clr->G, G);
    set_pixel_pwm(x,y,clr->B, B);
}

void set_white_pixel(unsigned int x, unsigned int y)
{
    RGBLED white;
    white.R = 255;
    white.G = 255;
    white.B = 255;
    
    set_pixel(x, y, &white);
}

//Change memory page in the LED controller
BOOL set_page(unsigned int page, SLAVE_ADDR cont_addr)
{
    BOOL succ;
    //unlock page register
    succ = i2c_write_single(0xC5 , cont_addr, 0xFE);
    //set page
    succ &= i2c_write_single(page , cont_addr, 0xFD);
    return succ;
}

void get_random_color(RGBLED *clr)
{
    clr->R = (char) (rand() % 255);
    clr->G = (char) (rand() % 255);
    clr->B = (char) (rand() % 255);
}


//Draws a circle on the screen
void draw_circle(unsigned int x0, unsigned int y0, unsigned int radius, RGBLED *clr)
{
    unsigned int x = radius-1;
    unsigned int y = 0;
    unsigned int dx = 1;
    unsigned int dy = 1;
    unsigned int err = dx - (radius << 1);

    while (x >= y)
    {
        set_pixel(x0 + x, y0 + y, clr);
        set_pixel(x0 + y, y0 + x, clr);
        set_pixel(x0 - y, y0 + x, clr);
        set_pixel(x0 - x, y0 + y, clr);
        set_pixel(x0 - x, y0 - y, clr);
        set_pixel(x0 - y, y0 - x, clr);
        set_pixel(x0 + y, y0 - x, clr);
        set_pixel(x0 + x, y0 - y, clr);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += (-radius << 1) + dx;
        }
    }
}


//Function called by the interrupt to updated the LEDs
void screen_update(TIMEDAT *time)
{
    //Display circles when time has to be set
    if(time_needs_to_be_set())
        set_load_led();
    //Set time if the time is valid
    else
        set_time_led(time);
    
    refresh_led();
    
    SCR_UPD_INT = FALSE;
}

//Function sets the time given on the LEDs
void set_time_led(TIMEDAT *time)
{
    clear_led();
    set_die_tyd_is();
    int min = time->minute;
    int hour = time->hour;
    
    BOOL teen = FALSE;
    
    if(min > 30)
    {
        hour--;
        min = 60 - min;
        set_voor();
    }
    else if (min < 30)
    {
        set_oor();
    }
    if(time->minute == 1)
    {
        set_minuut();
        set_uur();
    }
    else if(time->minute != 30)
    {
        set_minute();
        set_uur();
    }
    
    if(min > 10 && min < 20)
    {
        min -= 10;
        teen = TRUE;
        switch(min)
        {
            case 1:
                set_elf_min();
                break;
            case 2:
                set_twaalf_min();
                break;
            case 3:
                set_der();
                set_tien_min();
                break;
            case 4:
                set_veer();
                set_tien_min();
                break;
            case 5:
                set_kwart();
                break;
            case 7:
                set_ntien_min();
                break;
            case 9:
                set_ntien_min();
            default:
                set_tien_min();
                break;       
        }
    }else if(min >= 20 && min < 30)
    {
        min -= 20;
        set_twintig();
        if(min > 0)
            set_en();
    }else
    {
        min -= 30;
        set_half();
    }
            
    switch(min)
    {
        case 1:
            if(!teen)
                set_een_min();
            break;
        case 2:
            if(!teen)
                set_twee_min();
            break;
        case 3:
            if(!teen)
                set_drie_min();
            break;
        case 4:
            if(!teen)
                set_vier_min();
            break;
        case 5:
            if(!teen)
                set_vyf_min();
            break;
        case 6:
            set_ses_min();
            break;
         case 7:
            set_sewe_min();
            break;
         case 8:
            set_agt_min();
            break;
         case 9:
            set_nege_min();
            break;
         case 10:
            set_tien_min();
            break;  
        default:
            break;
    }
    
    switch(hour)
    {
        case 1:
            set_een_hour();
            break;
        case 2:
            set_twee_hour();
            break;
        case 3:
            set_drie_uur();
            break;
        case 4:
            set_vier_uur();
            break;
        case 5:
            set_vyf_uur();
            break;
        case 6:
            set_ses_uur();
            break;
         case 7:
            set_sewe_uur();
            break;
         case 8:
            set_agt_uur();
            break;
         case 9:
            set_nege_uur();
            break;
         case 10:
            set_tien_uur();
            break;  
         case 11:
            set_elf_uur();
            break; 
         case 12:
            set_twaalf_uur();
            break; 
         default:
            break;
    }
    
    if(time->day == 24 && time->month == 9)
    {
        set_gelukkige();
        set_verjaars();
        set_dag();
        set_minell();
    }
    
    if(time->day == 7 && time->month == 6)
    {
        set_gelukkige();
        set_verjaars();
        set_dag();
        set_jason();
    }
    
    if(time->day == 14 && time->month == 6)
    {
        set_gelukkige();
        set_trou();
        set_dag();
        set_minell();
        set_en_mj();
        set_jason();
    }
            
}


//Display random circles
void set_load_led(void)
{
    if(counter++ > MAX_COUNTER)
    {
        clear_led();
        unsigned int x = (unsigned int)(rand() % 15 + 0);     //x random between 1 and 16
        unsigned int y = (unsigned int)(rand() % 11 + 0);     //y random between 1 and 12
        unsigned int radius = (unsigned int)(rand() % 5 + 1); //radius random between 1 and 5
        RGBLED color;
        get_random_color(&color);
        draw_circle(x, y, radius, &color);
    }
    
    if(counter > MAX_COUNTER)
        counter == 0;
}


void set_die_tyd_is(void)
{
    set_white_pixel(1,12);
    set_white_pixel(2,12);
    set_white_pixel(3,12);
    
    set_white_pixel(5,12);
    set_white_pixel(6,12);
    set_white_pixel(7,12);
    
    set_white_pixel(9,12);
    set_white_pixel(10,12);
}


void set_kwart(void)
{
    set_white_pixel(12,12);
    set_white_pixel(13,12);
    set_white_pixel(14,12);
    set_white_pixel(15,12);
    set_white_pixel(16,12);
}

void set_half(void)
{
    set_white_pixel(1,11);
    set_white_pixel(2,11);
    set_white_pixel(3,11);
    set_white_pixel(4,11);
}

void set_twee_min(void)
{
    set_white_pixel(5,11);
    set_white_pixel(6,11);
    set_white_pixel(7,11);
    set_white_pixel(8,11);
}

void set_drie_min(void)
{
    set_white_pixel(10,11);
    set_white_pixel(11,11);
    set_white_pixel(12,11);
    set_white_pixel(13,11);
}

void set_een_min(void) 
{
    set_white_pixel(7,11);
    set_white_pixel(8,11);
    set_white_pixel(9,11);
}
   
void set_vier_min(void) 
{
    set_white_pixel(1,10);
    set_white_pixel(2,10);
    set_white_pixel(3,10);
    set_white_pixel(4,10);
}
  
void set_vyf_min(void)
{
    set_white_pixel(14,11);
    set_white_pixel(15,11);
    set_white_pixel(16,11);
}

void set_ses_min(void)
{
    set_white_pixel(5,10);
    set_white_pixel(6,10);
    set_white_pixel(7,10);
}

void set_sewe_min(void)
{
    set_white_pixel(7,10);
    set_white_pixel(8,10);
    set_white_pixel(9,10);
    set_white_pixel(10,10);
}

void set_agt_min(void)
{
    set_white_pixel(14,9);
    set_white_pixel(15,9);
    set_white_pixel(16,9);
}

void set_nege_min(void)
{
    set_white_pixel(13,10);
    set_white_pixel(14,10);
    set_white_pixel(15,10);
    set_white_pixel(16,10);
}

void set_tien_min(void)
{
    set_white_pixel(13,8);
    set_white_pixel(14,8);
    set_white_pixel(15,8);
    set_white_pixel(16,8);
}

void set_ntien_min(void)
{
    set_white_pixel(12,8);
    set_white_pixel(13,8);
    set_white_pixel(14,8);
    set_white_pixel(15,8);
    set_white_pixel(16,8);
}

void set_elf_min(void)
{
    set_white_pixel(10,10);
    set_white_pixel(11,10);
    set_white_pixel(12,10);
}

void set_twaalf_min(void)
{
    set_white_pixel(6,8);
    set_white_pixel(7,8);
    set_white_pixel(8,8);
    set_white_pixel(9,8);
    set_white_pixel(10,8);
    set_white_pixel(11,8);
}

void set_der(void)
{
    set_white_pixel(11,9);
    set_white_pixel(12,9);
    set_white_pixel(13,9);
}

void set_veer(void)
{
    set_white_pixel(1,8);
    set_white_pixel(2,8);
    set_white_pixel(3,8);
    set_white_pixel(4,8);
}

void set_twintig(void)
{
    set_white_pixel(10,7);
    set_white_pixel(11,7);
    set_white_pixel(12,7);
    set_white_pixel(13,7);
    set_white_pixel(14,7);
    set_white_pixel(15,7);
    set_white_pixel(16,7);
}

void set_en(void)
{
    set_white_pixel(15,8);
    set_white_pixel(16,8);
}

void set_minuut(void)
{
    set_white_pixel(1,5);
    set_white_pixel(2,5);
    set_white_pixel(3,5);
    set_white_pixel(4,5);
    set_white_pixel(5,5);
    set_white_pixel(6,5);
}

void set_minute(void)
{
    set_white_pixel(11,5);
    set_white_pixel(12,5);
    set_white_pixel(13,5);
    set_white_pixel(14,5);
    set_white_pixel(15,5);
    set_white_pixel(16,5);
}

void set_voor(void)
{
    set_white_pixel(7,4);
    set_white_pixel(8,4);
    set_white_pixel(9,4);
    set_white_pixel(10,4);
}

void set_oor(void)
{
    set_white_pixel(8,4);
    set_white_pixel(9,4);
    set_white_pixel(10,4);
}

void set_een_hour(void)
{
    set_white_pixel(1,3);
    set_white_pixel(2,3);
    set_white_pixel(3,3);
}

void set_twee_hour(void)
{
    set_white_pixel(12,4);
    set_white_pixel(13,4);
    set_white_pixel(14,4);
    set_white_pixel(15,4);
}

void set_drie_uur(void)
{
    set_white_pixel(13,3);
    set_white_pixel(14,3);
    set_white_pixel(15,3);
    set_white_pixel(16,3);
}

void set_vier_uur(void)
{
    set_white_pixel(5,1);
    set_white_pixel(6,1);
    set_white_pixel(7,1);
    set_white_pixel(8,1);
}

void set_vyf_uur(void)
{
    set_white_pixel(4,3);
    set_white_pixel(5,3);
    set_white_pixel(6,3);
}

void set_ses_uur(void)
{
    set_white_pixel(7,3);
    set_white_pixel(8,3);
    set_white_pixel(9,3);
}

void set_sewe_uur(void)
{
    set_white_pixel(9,3);
    set_white_pixel(10,3);
    set_white_pixel(11,3);
    set_white_pixel(12,3);
}

void set_agt_uur(void)
{
    set_white_pixel(2,2);
    set_white_pixel(3,2);
    set_white_pixel(4,2);
}

void set_nege_uur(void)
{
    set_white_pixel(13,2);
    set_white_pixel(14,2);
    set_white_pixel(15,2);
    set_white_pixel(16,2);
}

void set_tien_uur(void)
{
    set_white_pixel(9,1);
    set_white_pixel(10,1);
    set_white_pixel(11,1);
    set_white_pixel(12,1);
}

void set_elf_uur(void)
{
    set_white_pixel(1,1);
    set_white_pixel(2,1);
    set_white_pixel(3,1);
}

void set_twaalf_uur(void)
{
    set_white_pixel(6,2);
    set_white_pixel(7,2);
    set_white_pixel(8,2);
    set_white_pixel(9,2);
    set_white_pixel(10,2);
    set_white_pixel(11,2);
}

void set_uur(void)
{
    set_white_pixel(14,1);
    set_white_pixel(15,1);
    set_white_pixel(16,1);
}

void set_gelukkige(void)
{
    RGBLED clr;
    get_random_color(&clr);

    set_pixel(1, 9, &clr);
    set_pixel(2, 9, &clr);
    set_pixel(3, 9, &clr);
    set_pixel(4, 9, &clr);
    set_pixel(5, 9, &clr);
    set_pixel(6, 9, &clr);
    set_pixel(7, 9, &clr);
    set_pixel(8, 9, &clr);
    set_pixel(9, 9, &clr);
}

void set_verjaars(void)
{
    RGBLED clr;
    get_random_color(&clr);

    set_pixel(1, 7, &clr);   
    set_pixel(2, 7, &clr);  
    set_pixel(3, 7, &clr);  
    set_pixel(4, 7, &clr);  
    set_pixel(5, 7, &clr);  
    set_pixel(6, 7, &clr);  
    set_pixel(7, 7, &clr);  
    set_pixel(8, 7, &clr);  
}

void set_trou(void)
{
    RGBLED clr;
    get_random_color(&clr);

    set_pixel(3, 6, &clr); 
    set_pixel(4, 6, &clr); 
    set_pixel(5, 6, &clr); 
    set_pixel(6, 6, &clr); 
}

void set_dag(void)
{
    RGBLED clr;
    get_random_color(&clr);

    set_pixel(2, 6, &clr);  
    set_pixel(3, 6, &clr);
    set_pixel(4, 6, &clr);
}

void set_minell(void)
{
    RGBLED clr;
    get_random_color(&clr);

    set_pixel(11, 6, &clr); 
    set_pixel(12, 6, &clr);  
    set_pixel(13, 6, &clr);  
    set_pixel(14, 6, &clr);  
    set_pixel(15, 6, &clr);  
    set_pixel(16, 6, &clr);  
}

void set_en_mj(void)
{
    RGBLED clr;
    get_random_color(&clr);

    set_pixel(8, 5, &clr);
    set_pixel(9, 5, &clr); 
}

void set_jason(void) 
{
    RGBLED clr;
    get_random_color(&clr);

    set_pixel(1, 4, &clr);
    set_pixel(2, 4, &clr);
    set_pixel(3, 4, &clr);
    set_pixel(4, 4, &clr);
    set_pixel(5, 4, &clr);
    
}
       