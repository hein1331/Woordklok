#ifndef LED_H
#define	LED_H

#include "stdtypes.h"
#include "clock.h"

enum
{
    R = 0,
    G = 2, 
    B = 1,
};

#define SCR_UPD_INT         PIR4bits.TMR2IF

#define PRESCALAR_LED       128.0f

#define REXT_KOHM           33

unsigned int LED_TIM_VAL;

typedef struct
{
    char R;
    char G;
    char B;
} RGBLED;

#define LED_CNT_CONT    192
#define BOARD_CNT       3

#define MAX_COUNTER     5


//Functions
void init_led(void);
BOOL refresh_led(void);
void set_pixel_pwm(unsigned int x, unsigned int y, char pwm, unsigned int clr);
void set_pixel(unsigned int x, unsigned int y, RGBLED *clr);
void set_white_pixel(unsigned int x, unsigned int y);
void get_random_color(RGBLED *clr);
BOOL set_page(unsigned int page, SLAVE_ADDR cont_addr);
void clear_led(void);
void screen_update(TIMEDAT *time);
void draw_circle(unsigned int x0, unsigned int y0, unsigned int radius, RGBLED *clr);
void set_load_led(void);
void set_time_led(TIMEDAT *time);
void set_led_current(unsigned int curr_ma, int board);
void open_short_test(int board);

void set_die_tyd_is(void);
void set_kwart(void);
void set_half(void);
void set_twee_min(void);
void set_drie_min(void);
void set_een_min(void);   
void set_vier_min(void);   
void set_vyf_min(void);
void set_ses_min(void);
void set_sewe_min(void);
void set_agt_min(void);
void set_nege_min(void);
void set_tien_min(void);
void set_ntien_min(void);
void set_elf_min(void);
void set_twaalf_min(void);
void set_der(void);
void set_veer(void);
void set_twintig(void);
void set_en(void);
void set_minuut(void);
void set_minute(void);
void set_voor(void);
void set_oor(void);
void set_een_hour(void);
void set_twee_hour(void);
void set_drie_uur(void);
void set_vier_uur(void);
void set_vyf_uur(void);
void set_ses_uur(void);
void set_sewe_uur(void);
void set_agt_uur(void);
void set_nege_uur(void);
void set_tien_uur(void);
void set_elf_uur(void);
void set_twaalf_uur(void);
void set_uur(void);
void set_gelukkige(void);
void set_verjaars(void);
void set_trou(void);
void set_dag(void);
void set_minell(void);
void set_en_mj(void);
void set_jason(void);
#endif	/* LED_H */

