/*******************************************************************************
 * @name    : OLED显示实验
 * @author  : 布谷鸟
 * @web     : WWW.UCORTEX.COM
 * @version : V1.0
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : 通过4线SPI驱动OLED显示
 * ---------------------------------------------------------------------------- 
 * @copyright
 *
 * UCORTEX版权所有，违者必究！例程源码仅供大家参考，旨在交流学习和节省开发时间，
 * 对于因为参考本文件内容，导致您的产品直接或间接受到破坏，或涉及到法律问题，作
 * 者不承担任何责任。对于使用过程中发现的问题，如果您能在WWW.UCORTEX.COM网站反
 * 馈给我们，我们会非常感谢，并将对问题及时研究并作出改善。例程的版本更新，将不
 * 做特别通知，请您自行到WWW.UCORTEX.COM下载最新版本，谢谢。
 * 对于以上声明，UCORTEX保留进一步解释的权利！
 * ----------------------------------------------------------------------------
 * @description
 * 
 * 本实验通过4线驱动OLED，在屏幕上周期性显示字符和对应的ASCII码。
 *
 * OLED_PWR  -> PC13 : OLED电源控制口，高电平打开电源
 * OLED_CS   -> PB3  : OLED SPI选通
 * OLED_RST  -> PB4  : OLED复位
 * OLED_DC   -> PB5  : OLED 数据/命令选择口 D/C#
 * OLED_SCLK -> PB6  : OLED SPI时钟
 * OLED_SDIN -> PB7  : OLED SPI数据
 *
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------
 * 更改时间：2014-04-03    更改人：布谷鸟
 * 版本记录：V1.0
 * 更改内容：新建
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
	LED_Init();		//LED IO初始化
	KEY_Init();		//按键IO初始化
//	COM_Init(COM1, 115200);//串口初始化
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
	

	//OLED_Init();	//初始化OLED
	
	//OLED显示提示信息
	
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
	
				if(KEY_Scan(0))//检测到按键按下
		{
				hott_msg_len=30;
			hott_msg_ptr=test_serial_p;
			send_one_frame_data();
//			printf("WWW.UCORTEX.COM\r\n");
		}
/*			
		if(COM_RX_STA&0x8000)//接收到有效数据
		{
			len = COM_RX_STA & 0x3fff;//串口接收到的数据长度
			for(i = 0; i < len; i++)
			{
				COM_RX_BUF[len]='\0';
				OLED_ShowString(0,32,COM_RX_BUF); 
				USART_SendData(USART1, COM_RX_BUF[i]);//串口发送一个数据
				while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//等待串口发送完成
			}
			printf("\r\n");//打印回车换行
			COM_RX_STA = 0;//串口接收状态字清零
		}
		*/
	
		OLED_ShowChar(48,48,t,16,1);//显示ASCII字符	   
		OLED_Refresh_Gram();
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,48,t,3,16);//显示ASCII字符的码值 
		delay_ms(300);
		LED = !LED;
		
	}	

}

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
