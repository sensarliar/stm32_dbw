/*
 *
 * Copyright (C) 2008-2011 The Paparazzi Team
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

/**
 * @file gps_nmea.c
 * Parser for the NMEA protocol.
 *
 * TODO: THIS NMEA-PARSER IS NOT WELL TESTED AND INCOMPLETE!!!
 * Status:
 *  Parsing GGA and RMC is complete, GSA and other records are
 *  incomplete.
 */

#include "gps.h"
#include "gps_nmea.h"

#include "led.h"

#if GPS_USE_LATLONG
/* currently needed to get nav_utm_zone0 */
#include "subsystems/navigation/common_nav.h"
#endif
//#include "math/pprz_geodetic_float.h"

#include <inttypes.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#ifdef DEBUG_NMEA
// do debug-output if run on the DEBUG_NMEA-target

#endif


struct GpsNmea gps_nmea;

//void parse_nmea_GPGSA(void);
void parse_nmea_GPRMC(void);
void parse_nmea_GPGGA(void);


void gps_impl_init( void ) {
  gps_nmea.msg_available = FALSE;
  gps_nmea.pos_available = FALSE;
  gps_nmea.gps_nb_ovrn = 0;
  gps_nmea.msg_len = 0;
}




/**
 * parse GPRMC-nmea-messages stored in
 * gps_nmea.msg_buf .
 */
void parse_nmea_GPRMC(void) {
  int i = 6;     // current position in the message, start after: GPRMC,
//  char* endptr;  // end of parsed substrings
	int j=0;
  // attempt to reject empty packets right away
  if(gps_nmea.msg_buf[i]==',' && gps_nmea.msg_buf[i+1]==',') {
    NMEA_PRINT("p_GPRMC() - skipping empty message\n\r");
    return;
  }

  // get time
  // ignored
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: warning
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPRMC() - skipping incomplete message\n\r");
      return;
    }
  }

  // get warning
  // ignored
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: lat
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPRMC() - skipping incomplete message\n\r");
      return;
    }
  }
  // get lat
  // ignored
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: N/S
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPRMC() - skipping incomplete message\n\r");
      return;
    }
  }
  // get North/South
  // ignored
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: lon
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPRMC() - skipping incomplete message\n\r");
      return;
    }
  }
  // get lon
  // ignored
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: E/W
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPRMC() - skipping incomplete message\n\r");
      return;
    }
  }
  // get eath/west
  // ignored
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: speed
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPRMC() - skipping incomplete message\n\r");
      return;
    }
  }
  // get speed
//  double speed = strtod(&gps_nmea.msg_buf[i], &endptr);
//  gps.gspeed = speed * 1.852 * 100 / (60*60);
			  j=0;
  NMEA_PRINT("p_GPRMC() - ground-speed=%d knot = %d cm/s\n\r", (speed*1000), (gps.gspeed*1000));
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: speed
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPRMC() - skipping incomplete message\n\r");
      return;
    }
		gps.speed_ch[j++]=gps_nmea.msg_buf[i-1];
  }
	  gps.speed_ch[j]='\0';


	
	
	
//  double course = strtod(&gps_nmea.msg_buf[i], &endptr);
//  gps.course = RadOfDeg(course) * 1e7;
  NMEA_PRINT("COURSE: %d \n\r",gps_course);
	
	  while(gps_nmea.msg_buf[i++] != ',') {              // next field: trak true
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPRMC() - skipping incomplete message\n\r");
      return;
    }
  }
		//ddmmyy
	
	  j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: date
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.date_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.date_ch[j]='\0';
	
}

/**
 * parse GPGGA-nmea-messages stored in
 * gps_nmea.msg_buf .
 */
void parse_nmea_GPGGA(void) {
  int i = 6;     // current position in the message, start after: GPGGA,
//  char* endptr;  // end of parsed substrings
	int j=0;
//  double degrees, minutesfrac;
//  struct LlaCoor_f lla_f;

  // attempt to reject empty packets right away
  if(gps_nmea.msg_buf[i]==',' && gps_nmea.msg_buf[i+1]==',') {
    NMEA_PRINT("p_GPGGA() - skipping empty message\n\r");
    return;
  }

  // get UTC time [hhmmss.sss]
  // ignored GpsInfo.PosLLA.TimeOfFix.f = strtod(&packet[i], &endptr);
  // FIXME: parse UTC time correctly
  //double time = strtod(&gps_nmea.msg_buf[i],&endptr);
 // gps.tow = (uint32_t)((time+1)*1000);
  //gps.tow = time*1000;

  //AD TODO: strtod itow
  j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: time
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.time_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.time_ch[j]='\0';
  // get latitude [ddmm.mmmmm]
//  double lat = strtod(&gps_nmea.msg_buf[i], &endptr);
  // convert to pure degrees [dd.dddd] format
//  minutesfrac = modf(lat/100, &degrees);
 // lat = degrees + (minutesfrac*100)/60;
  // convert to radians
  //GpsInfo.PosLLA.lat.f *= (M_PI/180);
 // gps.lat = lat*10000;
    j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: latitude
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.lat_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.lat_ch[j]='\0';

  gps.lat_du_ch[0]=gps.lat_ch[0];
  gps.lat_du_ch[1]=gps.lat_ch[1];
  gps.lat_du_ch[2]='\0';
  /*gps.lat = strtod(&gps.lat_ch[5], &endptr);
  gps.lat = gps.lat*60/10000;
  gps.lat=(double((int)(gps.lat*100)))/100;
  //strtod();
*/
  gps.NorS = gps_nmea.msg_buf[i];
  // correct latitute for N/S
 // if(gps_nmea.msg_buf[i] == 'S')
 //   lat = -lat;

  // convert to radians
//  lla_f.lat = RadOfDeg(lat);

//  gps.lla_pos.lat = lla_f.lat * 1e7; // convert to fixed-point
  NMEA_PRINT("p_GPGGA() - lat=%d gps_lat=%i\n\r", (lat*1000), lla_f.lat);


  while(gps_nmea.msg_buf[i++] != ',') {              // next field: N / S
    if (i >= gps_nmea.msg_len)
      return;
  }
/*
  // get longitude [ddmm.mmmmm]
  double lon = strtod(&gps_nmea.msg_buf[i], &endptr);
  // convert to pure degrees [dd.dddd] format
//  minutesfrac = modf(lon/100, &degrees);
//  lon = degrees + (minutesfrac*100)/60;
  gps.lon = lon*10000;
  // convert to radians
  //GpsInfo.PosLLA.lon.f *= (M_PI/180);
  */
  j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: LOGITUDE
    if (i >= gps_nmea.msg_len)
      return;
    gps.lon_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.lon_ch[j]='\0';

  gps.lon_du_ch[0]=gps.lon_ch[0];
  gps.lon_du_ch[1]=gps.lon_ch[1];
  gps.lon_du_ch[2]=gps.lon_ch[2];
  gps.lon_du_ch[3]='\0';

    gps.EorW = gps_nmea.msg_buf[i];
  // correct latitute for E/W
 /* if(gps_nmea.msg_buf[i] == 'W')
    lon = -lon;
*/
  // convert to radians
//  lla_f.lon = RadOfDeg(lon);

//  gps.lla_pos.lon = lla_f.lon * 1e7; // convert to fixed-point
  NMEA_PRINT("p_GPGGA() - lon=%d gps_lon=%i time=%u\n\r", (lon*1000), lla_f.lon, gps.tow);


  while(gps_nmea.msg_buf[i++] != ',') {              // next field: E / W
    if (i >= gps_nmea.msg_len)
      return;
  }

  // position fix status
  // 0 = Invalid, 1 = Valid SPS, 2 = Valid DGPS, 3 = Valid PPS
  // check for good position fix
  if( (gps_nmea.msg_buf[i] != '0') && (gps_nmea.msg_buf[i] != ',') )  {
    gps_nmea.pos_available = TRUE;
    NMEA_PRINT("p_GPGGA() - POS_AVAILABLE == TRUE\n\r");
  } else {
    gps_nmea.pos_available = FALSE;
    NMEA_PRINT("p_GPGGA() - gps_pos_available == false\n\r");
  }

  while(gps_nmea.msg_buf[i++] != ',') {              // next field: satellites used
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r\r");
      return;
    }
  }
  // get number of satellites used in GPS solution
//  gps.num_sv = atoi(&gps_nmea.msg_buf[i]);
  NMEA_PRINT("p_GPGGA() - gps_numSatlitesUsed=%i\n\r", gps.num_sv);

  while(gps_nmea.msg_buf[i++] != ',') {              // next field: HDOP (horizontal dilution of precision)
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
  }
  // we use HDOP here, as the PDOP is not in the message
//  float hdop = strtof(&gps_nmea.msg_buf[i], &endptr);
 // gps.pdop = hdop * 100;
//  gps.pdop = hdop;

  while(gps_nmea.msg_buf[i++] != ',') {              // next field: altitude
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
  }
  // get altitude (in meters) above geoid (MSL)
  // lla_f.alt should actuall be height above ellipsoid,
  // but since we don't get that, use hmsl instead
 // lla_f.alt = strtof(&gps_nmea.msg_buf[i], &endptr);
//  float lla_f_alt = strtof(&gps_nmea.msg_buf[i], &endptr);
//  gps.hmsl = lla_f.alt * 1000;
  // gps.hmsl = lla_f_alt * 1000;
//  gps.alt = lla_f_alt;
//  gps.lla_pos.alt = gps.hmsl;
//  NMEA_PRINT("p_GPGGA() - gps_alt=%i\n\r", gps.hmsl);
 j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: altitude units, always 'M'
    if (i >= gps_nmea.msg_len)
      return;
   gps.alt_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.alt_ch[j]='\0';
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: geoid seperation
    if (i >= gps_nmea.msg_len)
      return;
  }

  //while(gps_nmea.msg_buf[i++] != '*');              // next field: checksum



}


/**
 * parse_nmea_char() has a complete line.
 * Find out what type of message it is and
 * hand it to the parser for that type.
 */
void nmea_parse_msg( void ) {

  if(gps_nmea.msg_len > 5 && !strncmp(gps_nmea.msg_buf , "GNRMC", 5)) {
    gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
    NMEA_PRINT("parsing RMC: \"%s\" \n\r",gps_nmea.msg_buf);
    NMEA_PRINT("RMC");
    parse_nmea_GPRMC();
  }
  else {
    if(gps_nmea.msg_len > 5 && !strncmp(gps_nmea.msg_buf , "GNGGA", 5)) {
      gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
      NMEA_PRINT("parse_gps_msg() - parsing GGA gps-message \"%s\" \n\r",gps_nmea.msg_buf);
      NMEA_PRINT("GGA");
      parse_nmea_GPGGA();
    }
    else {
      if(gps_nmea.msg_len > 5 && !strncmp(gps_nmea.msg_buf , "GPGSA", 5)) {
        gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
        NMEA_PRINT("GSA: \"%s\" \n\r",gps_nmea.msg_buf);
        NMEA_PRINT("GSA");
      //  parse_nmea_GPGSA();
      } else {
        gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
        NMEA_PRINT("ignoring: len=%i \n\r \"%s\" \n\r", gps_nmea.msg_len, gps_nmea.msg_buf);
      }
    }
  }

  // reset message-buffer
  gps_nmea.msg_len = 0;
}


/**
 * This is the actual parser.
 * It reads one character at a time
 * setting gps_nmea.msg_available to TRUE
 * after a full line.
 */
void nmea_parse_char( uint8_t c ) {
  //reject empty lines
  if (gps_nmea.msg_len == 0) {
    if (c == '\r' || c == '\n' || c == '$')
      return;
  }

  // fill the buffer, unless it's full
  if (gps_nmea.msg_len < NMEA_MAXLEN - 1) {

    // messages end with a linefeed
    //AD: TRUNK:       if (c == '\r' || c == '\n')
    if (c == '\r' || c == '\n') {
      gps_nmea.msg_available = TRUE;
    } else {
      gps_nmea.msg_buf[gps_nmea.msg_len] = c;
      gps_nmea.msg_len ++;
    }
  }

  if (gps_nmea.msg_len >= NMEA_MAXLEN - 1)
    gps_nmea.msg_available = TRUE;
}

void trans_disp_format(void)
{
	uint8_t hour_temp=0;
	if((gps.time_ch[0]>='0')&&(gps.time_ch[0]<='9')&&(gps.time_ch[1]>='0')&&(gps.time_ch[1]<='9'))
	{
		hour_temp = (gps.time_ch[0]-'0')*10+(gps.time_ch[1]-'0');
		hour_temp += 8;
		if(hour_temp>=24)
		{
			hour_temp %=24;
		}
		gps.time_disp[0] = (hour_temp/10) + '0';
		gps.time_disp[1] = (hour_temp%10) + '0';
		gps.time_disp[2] = ':';
		gps.time_disp[3] = gps.time_ch[2];
		gps.time_disp[4] = gps.time_ch[3];
		gps.time_disp[5] = ':';
		gps.time_disp[6] = gps.time_ch[4];
		gps.time_disp[7] = gps.time_ch[5];

		gps.time_disp[8] = gps.time_ch[6];
		gps.time_disp[9] = gps.time_ch[7];
		
		gps.time_disp[10] = gps.time_ch[8];
		gps.time_disp[11] = '\0';;
	}

	gps.date_disp[0] = '2';
	gps.date_disp[1] = '0';
	gps.date_disp[2] = gps.date_ch[4];
	gps.date_disp[3] = gps.date_ch[5];
	gps.date_disp[4] = '.';
	gps.date_disp[5] = gps.date_ch[2];
	gps.date_disp[6] = gps.date_ch[3];
	gps.date_disp[7] = '.';
	gps.date_disp[8] = gps.date_ch[0];
	gps.date_disp[9] = gps.date_ch[1];
  gps.date_disp[10] = '\0';
}
