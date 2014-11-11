#ifndef __UG25664_H__
#define __UG25664_H__



#include "stm32f10x.h"
#include "stdlib.h"
#include "bitband.h"

//OLED模式设置: 4线串行模式
		    						  
//-----------------OLED端口定义----------------
#define OLED_PWR			GPIOout(GPIOC,13)	//OLED电源控制IO，高电平打开电源
#define OLED_CS				GPIOout(GPIOB,3)	//OLED SPI选通
#define OLED_RST			GPIOout(GPIOB,4)	//OLED复位
#define OLED_DC				GPIOout(GPIOB,5)	//OLED D/C#
#define OLED_SCLK			GPIOout(GPIOB,6)	//OLED SPI时钟
#define OLED_SDIN			GPIOout(GPIOB,7)	//OLED SPI数据
 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数
void OLED_WR_Byte(uint8_t dat,uint8_t DC);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t dot);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
//void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p);
void OLED_ShowString(uint8_t x,uint8_t y,const char *p);
void OLED_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);

//#define OLED_PWR			//OLED电源控制IO，高电平打开电源
#define UGCS OLED_CS				//OLED SPI选通
#define UGRST OLED_RST			//OLED复位
#define UGA0 OLED_DC				//OLED D/C#
#define UGCK OLED_SCLK			//OLED SPI时钟
#define UGDI OLED_SDIN			//OLED SPI数据


#define	UGDI_1		UGDI=1;
#define	UGDI_0		UGDI=0;

#define	UGCK_1		UGCK=1;
#define	UGCK_0		UGCK=0;

#define	UGA0_1		UGA0=1;
#define	UGA0_0		UGA0=0;

#define	UGCS_1		UGCS=1;
#define	UGCS_0		UGCS=0;

#define	UGRST_1		UGRST=1;
#define	UGRST_0		UGRST=0;

void Initial_ssd1325(void);
void Write_Register(unsigned char cmdData);
void Write_Parameter(unsigned char Data);

void Set_Column_Address(unsigned char a, unsigned char b);
void Set_Row_Address(unsigned char a, unsigned char b);
void ClearLED(unsigned char color);
//void Fill(void);
void OLED_Refresh_Gram(void);

void Delay(unsigned int Data);

#endif

