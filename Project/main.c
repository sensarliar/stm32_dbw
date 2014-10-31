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

#include "ug25664.h"
#include "uart_arch.h"

//#define USE_UART1 1

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
	
	// HoTT serial send buffer pointer
static uint8_t *hott_msg_ptr = 0;
// Len of HoTT serial buffer
static int16_t hott_msg_len = 0;

uint8_t test_serial_p[43]="WWW.UCORTEX.COM---gaoming,gaoming,nihao ma";



static void send_one_frame_data(void) {
	
  static int16_t msg_crc = 0;
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
	while(hott_msg_len){
	    --hott_msg_len;
    if (hott_msg_len != 0) {
      msg_crc += *hott_msg_ptr;
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
	

	Initial_ssd1325();
		ClearLED(0);
		for(i=0; i<2000; i++);	
	

	
/*	
	while(1)
	{
	OLED_ShowString(0,0, "WWW.UCORTEX.COM");  
 	OLED_ShowString(0,16,"LEON @ UCORTEX");  
 	OLED_ShowString(0,32,"2014/03/23");  
 	OLED_ShowString(0,48,"ASCII:");  
 	OLED_ShowString(63,48,"CODE:");  

Fill();

	
	}  
	*/
/*		ClearLED(0);
		for(i=0; i<2000; i++);	
			ClearLED(7);
		for(i=0; i<2000; i++);

		ClearLED(15);
		for(i=0; i<2000; i++);


		Fill();
*/
	

	//OLED_Init();	//��ʼ��OLED
	
	//OLED��ʾ��ʾ��Ϣ
	
 	OLED_ShowString(0,0, "WWW.UCORTEX.COM---gaoming,gaoming,nihao ma");  

	OLED_ShowString(0,16,"LEON @ UCORTEX----nihao ^_^");  
 	OLED_ShowString(0,32,"2014/03/23");  
 	OLED_ShowString(0,48,"ASCII:");  
 	OLED_ShowString(63,48,"CODE:");  
	OLED_Refresh_Gram();
	


	t=' ';
	while(1) 
	{	
/*		
		    if (GpsBuffer()) {                                  \
      ReadGpsBuffer();                                  \
    }     
		
		*/
	
				if(KEY_Scan(0))//��⵽��������
		{
				hott_msg_len=30;
			hott_msg_ptr=test_serial_p;
			send_one_frame_data();
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
	
		OLED_ShowChar(48,48,t,16,1);//��ʾASCII�ַ�	   
		OLED_Refresh_Gram();
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,48,t,3,16);//��ʾASCII�ַ�����ֵ 
		delay_ms(300);
		LED = !LED;
		
	}	

}

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
