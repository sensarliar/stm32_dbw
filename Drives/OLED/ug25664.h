#ifndef __UG25664_H__
#define __UG25664_H__



#include "stm32f10x.h"
#include "stdlib.h"
#include "bitband.h"

//OLEDģʽ����: 4�ߴ���ģʽ
		    						  
//-----------------OLED�˿ڶ���----------------
#define OLED_PWR			GPIOout(GPIOC,13)	//OLED��Դ����IO���ߵ�ƽ�򿪵�Դ
#define OLED_CS				GPIOout(GPIOB,3)	//OLED SPIѡͨ
#define OLED_RST			GPIOout(GPIOB,4)	//OLED��λ
#define OLED_DC				GPIOout(GPIOB,5)	//OLED D/C#
#define OLED_SCLK			GPIOout(GPIOB,6)	//OLED SPIʱ��
#define OLED_SDIN			GPIOout(GPIOB,7)	//OLED SPI����
 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

//OLED�����ú���
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

//#define OLED_PWR			//OLED��Դ����IO���ߵ�ƽ�򿪵�Դ
#define UGCS OLED_CS				//OLED SPIѡͨ
#define UGRST OLED_RST			//OLED��λ
#define UGA0 OLED_DC				//OLED D/C#
#define UGCK OLED_SCLK			//OLED SPIʱ��
#define UGDI OLED_SDIN			//OLED SPI����


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

