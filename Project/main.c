/*******************************************************************************
 * @name    : OLED��ʾʵ��
 * @author  : ������
 * @web     : WWW.UCORTEX.COM
 * @version : V1.0
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : ͨ��4��SPI����OLED��ʾ
 * ---------------------------------------------------------------------------- 
 * @copyright
 *
 * UCORTEX��Ȩ���У�Υ�߱ؾ�������Դ�������Ҳο���ּ�ڽ���ѧϰ�ͽ�ʡ����ʱ�䣬
 * ������Ϊ�ο����ļ����ݣ��������Ĳ�Ʒֱ�ӻ����ܵ��ƻ������漰���������⣬��
 * �߲��е��κ����Ρ�����ʹ�ù����з��ֵ����⣬���������WWW.UCORTEX.COM��վ��
 * �������ǣ����ǻ�ǳ���л�����������⼰ʱ�о����������ơ����̵İ汾���£�����
 * ���ر�֪ͨ���������е�WWW.UCORTEX.COM�������°汾��лл��
 * ��������������UCORTEX������һ�����͵�Ȩ����
 * ----------------------------------------------------------------------------
 * @description
 * 
 * ��ʵ��ͨ��4������OLED������Ļ����������ʾ�ַ��Ͷ�Ӧ��ASCII�롣
 *
 * OLED_PWR  -> PC13 : OLED��Դ���ƿڣ��ߵ�ƽ�򿪵�Դ
 * OLED_CS   -> PB3  : OLED SPIѡͨ
 * OLED_RST  -> PB4  : OLED��λ
 * OLED_DC   -> PB5  : OLED ����/����ѡ��� D/C#
 * OLED_SCLK -> PB6  : OLED SPIʱ��
 * OLED_SDIN -> PB7  : OLED SPI����
 *
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-03    �����ˣ�������
 * �汾��¼��V1.0
 * �������ݣ��½�
 * ----------------------------------------------------------------------------
 *
 ******************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "oled.h"
#include "timer.h"

#include "ug25664.h"
#include "uart_arch.h"
#include "gps_nmea.h"
#include "gps.h"
struct GpsState gps;
//#define USE_UART1 1
extern uint8_t timer_60s_flag;

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
#define DBW_LINK UART1
	// HoTT serial send buffer pointer
static uint8_t *hott_msg_ptr = 0;
// Len of HoTT serial buffer
static int16_t hott_msg_len = 0;

//uint8_t MSG_TO_SEND[43]="WWW.UCORTEX.COM---gaoming,gaoming,nihao ma";
uint8_t INFO_HEAD[6]="$TXSQ";   //5B
uint8_t INFO_LEN[2]={0x33,0x34};  //2B
uint8_t INFO_ADDR[4]={0x04,0xE6,0x10};//321040  addr 3B
//uint8_t MSG_TX_FLAG=0B01000110;     //1B
uint8_t MSG_TX_FLAG=0x46;     //1B
uint8_t MSG_TX_ADDR[4]={0x04,0xE6,0x10};//321040  TX addr   //3B
uint8_t MSG_TX_LEN[2]={0x00,0x00};      //2B
uint8_t MSG_TX_ACK=0x00;               //1B
uint8_t MSG_TX[211]="gaoming";    //1680bit maxium   NON-MIL 628BIT  78B   17B+MSG +1CRC
uint16_t msg_num=3+9+11+12+5+6+1+1;

uint8_t flight_num_char[5]="007";

uint8_t ALL_HEAD[18];
		uint8_t	timer_flag1=0;
		uint8_t	timer_flag2=0;

void fill_msg(void){
		  int i,j;
			for(i=0,j=0; i<3; i++,j++){
			MSG_TX[i]=flight_num_char[j];
			}
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
			///bytes num =3+9+11+12+5+6+1+1
}

void init_msg_head(void){
	    int i,j;
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

static void send_one_frame_data(void) {
	
  static int16_t msg_crc = 0;
	int16_t init_len;

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

      uart_transmit(&uart1, *hott_msg_ptr++);
				
    } else
      uart_transmit(&uart1, (int8_t)msg_crc);
	}	
	
}
	
	
int main(void)
{
//	int len;
	uint8_t t;
	int i;
	LED_Init();		//LED IO��ʼ��
	KEY_Init();		//����IO��ʼ��
//	COM_Init(COM1, 115200);//���ڳ�ʼ��
	uart1_init();
	uart2_init();
//	TIM3_INT_Init(59999, 35999);
	TIM3_INT_Init(59999, 57599);
	

	Initial_ssd1325();
		ClearLED(0);
		for(i=0; i<2000; i++);	
	

		OLED_ShowString(0,0,"Time:");
		OLED_ShowString(128,0,"Date:");
		OLED_ShowString(0,16,"JD:");
		OLED_ShowString(0,32,"WD:");
		OLED_ShowString(0,48,"HT:");
		OLED_ShowString(80,48,"m");
		OLED_ShowString(128,48,"FLIGHT NUM: 007");

	t=' ';
	while(1) 
	{	
/*		
		    if (GpsBuffer()) {                                  \
      ReadGpsBuffer();                                  \
    }     
		
		*/
		
		GpsEvent(on_gps);

		OLED_ShowString(48,0,&gps.time_ch[0]);
//		OLED_ShowString(0,0,gps.time_ch);

		OLED_ShowString(176,0,&gps.date_ch[0]);
		if(gps_nmea.pos_available)
		{
			OLED_ShowString(32,16,&gps.lat_ch[0]);
			OLED_ShowString(32,32,&gps.lon_ch[0]);
			OLED_ShowString(32,48,&gps.alt_ch[0]);
			OLED_ShowChar(128,16,gps.NorS,16,1);
			OLED_ShowChar(136,32,gps.EorW,16,1);
		}else
		{
			OLED_ShowString(32,16,"               ");
			OLED_ShowString(32,32,"               ");
			OLED_ShowString(32,48,"     ");
		}
if((gps.time_ch[4]=='5')&&(gps.time_ch[5]=='4'))
{
	timer_flag1=1;
}
if((gps.time_ch[4]=='5')&&(gps.time_ch[5]=='5'))
{
	timer_flag2=1;
}

/*	
		if(gps_nmea.msg_available)
		{
				gps_nmea.msg_buf[gps_nmea.msg_len]='\0';
				OLED_ShowString(0,32,gps_nmea.msg_buf); 
		}		
	*/
//				if(KEY_Scan(0))//��⵽��������
//		if(timer_60s_flag)
//		if(timer_60s_flag)
	if((timer_flag1==1)&&(timer_flag2==1))
		{
			timer_flag1=0;
			timer_flag2=0;
				hott_msg_len=17+1+msg_num+1;
			  INFO_LEN[1]=(char)(hott_msg_len);
//  			INFO_LEN[1]=(char)*(hott_msg_len&0xFF00);
			INFO_LEN[0]= hott_msg_len>>8;
			MSG_TX_LEN[1]=(msg_num+1)*8;
			MSG_TX_LEN[0]=(uint16_t)(msg_num+1)*8 >> 8;
				init_msg_head();
			fill_msg();
			hott_msg_ptr=ALL_HEAD;
			send_one_frame_data();
			timer_60s_flag=0;

//			printf("WWW.UCORTEX.COM\r\n");
		}

/*			
		if(COM_RX_STA&0x8000)//���յ���Ч����
		{
			len = COM_RX_STA & 0x3fff;//���ڽ��յ������ݳ���
			for(i = 0; i < len; i++)
			{
				COM_RX_BUF[len]='\0';
				OLED_ShowString(0,32,COM_RX_BUF); 
				USART_SendData(USART1, COM_RX_BUF[i]);//���ڷ���һ������
				while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//�ȴ����ڷ������
			}
			printf("\r\n");//��ӡ�س�����
			COM_RX_STA = 0;//���ڽ���״̬������
		}
		*/
/*
		OLED_ShowChar(48,48,t,16,1);//��ʾASCII�ַ�	   
		OLED_Refresh_Gram();
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,48,t,3,16);//��ʾASCII�ַ�����ֵ 
		delay_ms(300);
*/
		OLED_Refresh_Gram();
//		LED = !LED;
		
	}	

}

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
