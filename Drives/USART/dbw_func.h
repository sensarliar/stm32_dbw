
#include "inc_types.h"

#define DBW_LINK UART2
	// HoTT serial send buffer pointer

extern uint8_t *hott_msg_ptr;
// Len of HoTT serial buffer
extern  int16_t hott_msg_len;

/*
extern uint8_t MSG_TX[211];    //1680bit maxium   NON-MIL 628BIT  78B   17B+MSG +1CRC
extern uint16_t msg_num;
*/

extern uint8_t ALL_HEAD[18];


extern void fill_msg(void);

extern void init_msg_head(void);


extern void on_gps(void);

extern void send_one_frame_data(void);
