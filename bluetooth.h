
#ifndef BLUETOOTH_H
#define	BLUETOOTH_H

#include "stdtypes.h"

void init_bt(void);
void bt_process(void);
BOOL event_compare(char* event_sent, const char *event_cmpr, int len);
int str_to_int(char* str_to_convert, int chars_to_convert);
BOOL get_bt_connected(void);

void send_status(void);
void send_tone(void);

BOOL bt_str_is_PAIR_OK(void);
BOOL bt_str_is_LINK_LOSS(void);
BOOL bt_str_is_CLOSE_OK(void);
BOOL bt_str_is_RECV_SPP(void);
BOOL bt_str_is_PAIR_ERROR(void);
BOOL bt_str_is_AVRCP_PLAY(void);
BOOL bt_str_is_AVRCP_STOP(void);
BOOL bt_str_is_AVRCP_PAUSE(void);

void send_Music_Playing(void);
void send_Music_Paused(void);

#endif	/* BLUETOOTH_H */

