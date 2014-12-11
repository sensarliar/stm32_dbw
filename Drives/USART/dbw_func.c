#include "dbw_func.h"
#include "gps.h"
#include "usart.h"
#include "uart_arch.h"
#include "plane_info.h"

//#define DBW_LINK UART1
	// HoTT serial send buffer pointer
uint8_t *hott_msg_ptr = 0;
// Len of HoTT serial buffer
int16_t hott_msg_len = 0;

//uint8_t MSG_TO_SEND[43]="WWW.UCORTEX.COM---gaoming,gaoming,nihao ma";
uint8_t INFO_HEAD[6]="$TXSQ";   //5B
uint8_t INFO_LEN[2]={0x33,0x34};  //2B
uint8_t INFO_ADDR[4]={0x04,0xE6,0x10};//321040  addr 3B
//uint8_t MSG_TX_FLAG=0B01000110;     //1B
uint8_t MSG_TX_FLAG=0x46;     //1B
//uint8_t MSG_TX_ADDR[4]={0x04,0xE6,0x10};//321040  TX addr   //3B
uint8_t MSG_TX_ADDR[4]={0x04,0x73,0xEE};//291822  TX addr   //3B
uint8_t MSG_TX_LEN[2]={0x00,0x00};      //2B
uint8_t MSG_TX_ACK=0x00;               //1B
uint8_t MSG_TX[211]="gaoming";    //1680bit maxium   NON-MIL 628BIT  78B   17B+MSG +1CRC
uint16_t msg_num=9+11+12+5+6+1+1;  //neirong

extern char flight_num_char[10];
uint8_t angle_roll[10]="0";
uint8_t angle_pitch[10]="0";
uint8_t angle_yaw[10]="0";

uint8_t ALL_HEAD[18];
extern struct GpsState gps;


void fill_msg(void){
		  int i,j;
			msg_num=9+11+12+5+6+1+1;
			i=0;
			for(j=0; j<9; i++,j++){
			MSG_TX[i]=gps.time_ch[j];
			}
			for(j=0; j<11; i++,j++){
			MSG_TX[i]=gps.lat_ch[j];
			}
			MSG_TX[i]=gps.NorS;
			i++;
			for(j=0; j<12; i++,j++){
			MSG_TX[i]=gps.lon_ch[j];
			}
			MSG_TX[i]=gps.EorW;
			i++;
			for(j=0; j<5; i++,j++){
			MSG_TX[i]=gps.alt_ch[j];
			}
			for(j=0; j<6; i++,j++){
			MSG_TX[i]=gps.date_ch[j];
			}
			
			j=0;
			while(gps.speed_ch[j])
			{
				MSG_TX[i++]=gps.speed_ch[j++];
			}

			MSG_TX[i]=',';
			i++;
			j++;
			msg_num += j;
			
			
			j=0;
			while(flight_num_char[j])
			{
				MSG_TX[i++]=flight_num_char[j++];
			}
			MSG_TX[i]=',';
			i++;
			j++;
			msg_num += j;
			
			MSG_TX[i]=plane_info_flag;
			i++;
			
			MSG_TX[i]=',';
			i++;
			j++;
			msg_num += j;
			
			j=0;
			while(angle_roll[j])
			{
				MSG_TX[i++]=angle_roll[j++];
			}

			MSG_TX[i]=',';
			i++;
			j++;
			msg_num += j;
			
			j=0;
			while(angle_pitch[j])
			{
				MSG_TX[i++]=angle_pitch[j++];
			}
			MSG_TX[i]=',';
			i++;
			j++;
			msg_num += j;
			
			j=0;
			while(angle_yaw[j])
			{
				MSG_TX[i++]=angle_yaw[j++];
			}
			MSG_TX[i]=',';
			i++;
			j++;
			msg_num += j;
			
			
			///bytes num =3+1+9+11+12+5+6+1+1
}

void init_msg_head(void){
	    int i,j;
			hott_msg_len=17+1+msg_num+1;
			INFO_LEN[1]=(char)(hott_msg_len);
//  	INFO_LEN[1]=(char)*(hott_msg_len&0xFF00);
			INFO_LEN[0]= hott_msg_len>>8;
			MSG_TX_LEN[1]=(msg_num+1)*8;
			MSG_TX_LEN[0]=(uint16_t)(msg_num+1)*8 >> 8;
	
			for(i=0,j=0; i<5; i++,j++){
			ALL_HEAD[i]=INFO_HEAD[j];
			}
			for(j=0; j<2; i++,j++){
			ALL_HEAD[i]=INFO_LEN[j];
			}
			for(j=0; j<3; i++,j++){
			ALL_HEAD[i]=INFO_ADDR[j];
			}

			ALL_HEAD[i]=MSG_TX_FLAG;
			i++;
//			ALL_HEAD[i]=0x46;

			for(j=0; j<3; i++,j++){
			ALL_HEAD[i]=MSG_TX_ADDR[j];
			}			
			for(j=0; j<2; i++,j++){
			ALL_HEAD[i]=MSG_TX_LEN[j];
			}	
		
			ALL_HEAD[i]=MSG_TX_ACK;
			i++;	
			ALL_HEAD[i]=0xa4;
			i++;	
			
}


void on_gps(void) {
}

//static void send_one_frame_data(void) {
void send_one_frame_data(void) {
  static int8_t msg_crc = 0;
	int16_t init_len;
	uint8_t temp;

	/*
  if (hott_msg_len == 0) {
    hott_msg_ptr = 0;
//    hott_telemetry_is_sending = FALSE;
//    hott_telemetry_sendig_msgs_id = 0;
    msg_crc = 0;
  }
  else {
    --hott_msg_len;
    if (hott_msg_len != 0) {
      msg_crc += *hott_msg_ptr;
      uart_transmit(&uart1, *hott_msg_ptr++);
    } else
      uart_transmit(&uart1, (int8_t)msg_crc);
  }
	*/
	msg_crc = 0;
	init_len=hott_msg_len;
	while(hott_msg_len){
	    --hott_msg_len;
    if (hott_msg_len != 0) {
			if(hott_msg_len == init_len-19)
			{hott_msg_ptr = MSG_TX;}
			
//      msg_crc = msg_crc  *hott_msg_ptr;
			msg_crc ^= *hott_msg_ptr;

      uart_transmit(&uart2, *hott_msg_ptr++);
				
    } else
      uart_transmit(&uart2, (int8_t)msg_crc);
	}	
	temp=0x0d;
	uart_transmit(&uart2, temp);
	temp=0x0a;
	uart_transmit(&uart2, temp);	

}
