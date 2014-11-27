/** @file dbw_rd.h
 *
*/


#ifndef DBW_RD_H
#define DBW_RD_H

#include "uart_arch.h"


#ifdef DEBUG_NMEA
#define NMEA_PRINT(...) {  UsbSPrintString( __VA_ARGS__);};
#else
#define NMEA_PRINT(...) {};
#endif

#define DBW_MAXLEN 511

struct DbwInfo {
  bool_t msg_available;
//  bool_t pos_available;
//  uint8_t gps_nb_ovrn;        // number if incomplete nmea-messages
  char msg_buf[DBW_MAXLEN];  ///< buffer for storing one nmea-line
  int16_t msg_len;
	uint16_t set_len;
	uint16_t set_temp_len;
};


extern struct DbwInfo dbw_info;
extern uint8_t dbw_info_flag;

#define DBW_LINK UART2
/*
 * This part is used by the autopilot to read data from a uart
 */
#define __DbwInfo(dev, _x) dev##_x
#define _DbwInfo(dev, _x)  __DbwInfo(dev, _x)
#define DbwInfoLink(_x) _DbwInfo(DBW_LINK, _x)

#define DbwInfoBuffer() DbwInfoLink(ChAvailable())

#define ReadDbwInfoBuffer() {					\
    while (DbwInfoLink(ChAvailable())&&!dbw_info.msg_available)	\
      dbw_info_parse_char(DbwInfoLink(Getch()));			\
  }


#define DbwInfoEvent() {             \
    if (DbwInfoBuffer()) {                                  \
      ReadDbwInfoBuffer();                                  \
    }                                                   \
    if (dbw_info.msg_available) {                       \
      dbw_info_parse_msg();                                 \
      dbw_info.msg_available = FALSE;                   \
    }                                                   \
}	
	
/** The function to be called when a characted friom the device is available */
extern void dbw_info_parse_char(uint8_t c);

extern void dbw_info_parse_msg(void);
extern void dbw_info_impl_init( void );



#endif /* GPS_NMEA_H */
