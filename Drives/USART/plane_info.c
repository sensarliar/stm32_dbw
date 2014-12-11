

//#include "gps.h"
#include "plane_info.h"

//#include "led.h"



//#include <inttypes.h>
#include <string.h>
//#include <math.h>
//#include <stdlib.h>
#ifdef DEBUG_NMEA
// do debug-output if run on the DEBUG_NMEA-target

#endif

#define FLAG_FDJ_BIT 1
#define FLAG_WQ_BIT 5
#define FLAG_QLJ_BIT 2
#define FLAG_JY_BIT 0


struct PlaneInfo plane_info;
uint8_t plane_info_flag = 0x19;

extern char flight_num_char[10];
extern uint8_t angle_roll[10];
extern uint8_t angle_pitch[10];
extern uint8_t angle_yaw[10];




void parse_plane_info_GM001(void);


void plane_info_impl_init( void ) {
  plane_info.msg_available = FALSE;
//  plane_info.pos_available = FALSE;
//  plane_info.gps_nb_ovrn = 0;
  plane_info.msg_len = 0;
}



/**
 * parse GPGGA-nmea-messages stored in
 * plane_info.msg_buf .
 */
void parse_plane_info_GM001(void) {
  int i = 6;     // current position in the message, start after: GPGGA,
	int j = 0;

  if(plane_info.msg_buf[i]==',' && plane_info.msg_buf[i+1]==',') {
    NMEA_PRINT("p_GPGGA() - skipping empty message\n\r");
    return;
  }

	  j=0;
  while(plane_info.msg_buf[i++] != ',') {              // next field: time
    if (i >= plane_info.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    flight_num_char[j++]=plane_info.msg_buf[i-1];
  }
  flight_num_char[j]='\0';
	
  if( (plane_info.msg_buf[i] != '0') && (plane_info.msg_buf[i] != ',') )  {
   // plane_info.pos_available = TRUE;
		plane_info_flag = plane_info_flag | (1<<FLAG_FDJ_BIT);
    NMEA_PRINT("p_GPGGA() - POS_AVAILABLE == TRUE\n\r");
  } else {
		
  //  plane_info.pos_available = FALSE;
		plane_info_flag = plane_info_flag & ~(1<<FLAG_FDJ_BIT);
    NMEA_PRINT("p_GPGGA() - gps_pos_available == false\n\r");
  }

  while(plane_info.msg_buf[i++] != ',') {              // next field: satellites used
    if (i >= plane_info.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r\r");
      return;
    }
  }	
	
  if( (plane_info.msg_buf[i] != '0') && (plane_info.msg_buf[i] != ',') )  {
   // plane_info.pos_available = TRUE;
		plane_info_flag = plane_info_flag | (1<<FLAG_WQ_BIT);
    NMEA_PRINT("p_GPGGA() - POS_AVAILABLE == TRUE\n\r");
  } else {
  //  plane_info.pos_available = FALSE;
		plane_info_flag = plane_info_flag & (~(1<<FLAG_WQ_BIT));
    NMEA_PRINT("p_GPGGA() - gps_pos_available == false\n\r");
  }
	
	  while(plane_info.msg_buf[i++] != ',') {              // next field: satellites used
    if (i >= plane_info.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r\r");
      return;
    }
  }	

		  j=0;
  while(plane_info.msg_buf[i++] != ',') {              // next field: time
    if (i >= plane_info.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    angle_roll[j++]=plane_info.msg_buf[i-1];
  }
  angle_roll[j]='\0';
	
			  j=0;
  while(plane_info.msg_buf[i++] != ',') {              // next field: time
    if (i >= plane_info.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    angle_pitch[j++]=plane_info.msg_buf[i-1];
  }
  angle_pitch[j]='\0';
	
			  j=0;
  while(plane_info.msg_buf[i++] != ',') {              // next field: time
    if (i >= plane_info.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    angle_yaw[j++]=plane_info.msg_buf[i-1];
  }
  angle_yaw[j]='\0';
	
//END parse GGA
}


/**
 * parse_nmea_char() has a complete line.
 * Find out what type of message it is and
 * hand it to the parser for that type.
 */
void plane_info_parse_msg( void ) {

  if(plane_info.msg_len > 5 && !strncmp(plane_info.msg_buf , "GMING", 5)) {
    plane_info.msg_buf[plane_info.msg_len] = 0;

		parse_plane_info_GM001();
  //  parse_nmea_GPRMC();
  }
  else {
    
        plane_info.msg_buf[plane_info.msg_len] = 0;
        NMEA_PRINT("ignoring: len=%i \n\r \"%s\" \n\r", plane_info.msg_len, plane_info.msg_buf);
      }

  // reset message-buffer
  plane_info.msg_len = 0;
}


/**
 * This is the actual parser.
 * It reads one character at a time
 * setting plane_info.msg_available to TRUE
 * after a full line.
 */
void plane_info_parse_char( uint8_t c ) {
  //reject empty lines
  if (plane_info.msg_len == 0) {
    if (c == '\r' || c == '\n' || c == '$')
      return;
  }

  // fill the buffer, unless it's full
  if (plane_info.msg_len < BD_MAXLEN - 1) {

    // messages end with a linefeed
    //AD: TRUNK:       if (c == '\r' || c == '\n')
    if (c == '\r' || c == '\n') {
      plane_info.msg_available = TRUE;
    } else {
      plane_info.msg_buf[plane_info.msg_len] = c;
      plane_info.msg_len ++;
    }
  }

  if (plane_info.msg_len >= BD_MAXLEN - 1)
    plane_info.msg_available = TRUE;
}
