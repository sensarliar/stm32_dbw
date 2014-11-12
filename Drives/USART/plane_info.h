

/** @file gps_nmea.h
 * NMEA protocol specific code.
 *
*/


#ifndef PLANE_INFO_H
#define PLANE_INFO_H

#include "uart_arch.h"

//#define GPS_NB_CHANNELS 16

#ifdef DEBUG_NMEA
#define NMEA_PRINT(...) {  UsbSPrintString( __VA_ARGS__);};
#else
#define NMEA_PRINT(...) {};
#endif

#define BD_MAXLEN 127

struct PlaneInfo {
  bool_t msg_available;
//  bool_t pos_available;
//  uint8_t gps_nb_ovrn;        // number if incomplete nmea-messages
  char msg_buf[BD_MAXLEN];  ///< buffer for storing one nmea-line
  int msg_len;
};


extern struct PlaneInfo plane_info;
extern uint8_t plane_info_flag;

#define FLY_INFO UART3
/*
 * This part is used by the autopilot to read data from a uart
 */
#define __PlaneInfo(dev, _x) dev##_x
#define _PlaneInfo(dev, _x)  __PlaneInfo(dev, _x)
#define PlaneInfoLink(_x) _PlaneInfo(FLY_INFO, _x)

#define PlaneInfoBuffer() PlaneInfoLink(ChAvailable())

#define ReadPlaneInfoBuffer() {					\
    while (PlaneInfoLink(ChAvailable())&&!plane_info.msg_available)	\
      plane_info_parse_char(PlaneInfoLink(Getch()));			\
  }
/*
#define PlaneInfoEvent(_sol_available_callback) {             \
    if (PlaneInfoBuffer()) {                                  \
      ReadPlaneInfoBuffer();                                  \
    }                                                   \
    if (plane_info.msg_available) {                       \
      plane_info_parse_msg();                                 \
        _sol_available_callback();                      \
      plane_info.msg_available = FALSE;                   \
    }                                                   \
  }
*/

#define PlaneInfoEvent() {             \
    if (PlaneInfoBuffer()) {                                  \
      ReadPlaneInfoBuffer();                                  \
    }                                                   \
    if (plane_info.msg_available) {                       \
      plane_info_parse_msg();                                 \
      plane_info.msg_available = FALSE;                   \
    }                                                   \
}	
	
/** The function to be called when a characted friom the device is available */
extern void plane_info_parse_char(uint8_t c);

extern void plane_info_parse_msg(void);
extern void plane_info_impl_init( void );



#endif /* GPS_NMEA_H */
