/*******************************************************************************
 
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
#include "dbw_func.h"
#include "plane_info.h"

uint8_t	timer_flag1=0;
uint8_t	timer_flag2=0;
uint8_t sec_num_temp = 0;
uint8_t sec_cmp_base = 33;

struct GpsState gps;
//#define USE_UART1 1
extern uint8_t timer_60s_flag;


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

	
	
int main(void)
{
//	int len;
//	uint8_t t;
	int i;
	LED_Init();		//LED IO初始化
	KEY_Init();		//按键IO初始化
//	COM_Init(COM1, 115200);//串口初始化
	uart1_init();
	uart2_init();
	uart3_init();
//	TIM3_INT_Init(59999, 35999);
//	TIM3_INT_Init(59999, 57599);
	

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
		OLED_ShowString(160,32,"P_I:");
		
		gps_impl_init();
		plane_info_impl_init();

//	t=' ';
	while(1) 
	{	
/*		
		    if (GpsBuffer()) {                                  \
      ReadGpsBuffer();                                  \
    }     
		
		*/
		
		GpsEvent(on_gps);
//		PlaneInfoEvent();

		OLED_ShowString(48,0,&gps.time_ch[0]);
//		OLED_ShowString(0,0,gps.time_ch);

		OLED_ShowString(176,0,&gps.date_ch[0]);
		gps.info_flag1=(plane_info_flag&0x0f)+'0';
		gps.info_flag2=((plane_info_flag&0xf0)>>4)+'0';
//		OLED_ShowString(216,32,&gps.lon_ch[0]);
		OLED_ShowChar(200,16,gps.info_flag1,16,1);
		OLED_ShowChar(216,16,gps.info_flag2,16,1);
		
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
		
		if(gps_nmea.pos_available)
		{
			//if(('6'>=gps.time_ch[4]>='0')&&('9'>=gps.time_ch[5]>='0'))///bug
			if(('6'>=gps.time_ch[4])&&(gps.time_ch[4]>='0')&&('9'>=gps.time_ch[5])&&(gps.time_ch[5]>='0'))
			{
				sec_num_temp = (gps.time_ch[4]-'0')*10+(gps.time_ch[5]-'0');
			}else
			{
					sec_num_temp = 0;
			}
				
			if(sec_num_temp == sec_cmp_base%60)
			//if(sec_num_temp == sec_cmp_base)
			{
				timer_flag1=0;
				timer_flag2=0;
			}

					if(sec_num_temp == (sec_cmp_base+2)%60)
			//if(sec_num_temp == (sec_cmp_base+2))
			{
				timer_flag1=1;
			}
			if(sec_num_temp == (sec_cmp_base+4)%60)
			//		if(sec_num_temp == (sec_cmp_base+10))
			{
				timer_flag2=1;
			}
		}



//				if(KEY_Scan(0))//检测到按键按下
//		if(timer_60s_flag)
//		if(timer_60s_flag)
	if(gps_nmea.pos_available&&(timer_flag1==1)&&(timer_flag2==1))
		{


				init_msg_head();
			fill_msg();
			hott_msg_ptr=ALL_HEAD;
			send_one_frame_data();
			timer_60s_flag=0;
			
			sec_cmp_base+=1;
			timer_flag1=0;
			timer_flag2=0;


		}



		OLED_Refresh_Gram();
//		LED = !LED;
		
	}	

}

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
