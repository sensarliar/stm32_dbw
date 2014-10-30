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
//#define STM32F1
#define NVIC_IRQ_COUNT 0

#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "oled.h"

#include "ug25664.h"

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t t;
	int i;
	LED_Init();		//LED IO��ʼ��
	KEY_Init();		//����IO��ʼ��
//	COM_Init(COM1, 115200);//���ڳ�ʼ��
	

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

	OLED_ShowString(0,16,"LEON @ UCORTEX----nihao");  
 	OLED_ShowString(0,32,"2014/03/23");  
 	OLED_ShowString(0,48,"ASCII:");  
 	OLED_ShowString(63,48,"CODE:");  
	OLED_Refresh_Gram();
	


	t=' ';
	while(1) 
	{		
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
