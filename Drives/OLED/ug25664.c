//#include "reg52.h"
//#include "oled.h"
#include "ug25664.h"

//#include "oled.h"
//#include "stdlib.h"
#include "oledfont.h"  	 
//#include "delay.h"

#define GRAYSCALE 0x0F

//code unsigned char gImage_1[64][128];
unsigned char OLED_GRAM[128][64];


/**
  * @brief 画点
	* @param x: X轴坐标（0~127）
	* @param y: Y轴坐标（0~63 ）
	* @param dot: 1 填充；0 清空
  * @retval none
  * @note
  */ 
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t dot)
{
	if(x>255 || y>63) return;//超出范围了.
	if(x%2)
	{	
		if(dot)OLED_GRAM[x/2][y] = (OLED_GRAM[x/2][y]&0xF0)|GRAYSCALE;
		else OLED_GRAM[x/2][y] = OLED_GRAM[x/2][y]&0xF0;
	}else
	{
		if(dot)OLED_GRAM[x/2][y] = (OLED_GRAM[x/2][y]&0x0F)|(GRAYSCALE << 4);
		else OLED_GRAM[x/2][y] = OLED_GRAM[x/2][y]&0x0F;
	}

}

/**
  * @brief 在指定位置显示一个字符
	* @param x:（0~127）
	* @param y:（0~63 ）
	* @param size: 选择字体 16/12 
  * @param mode: 0 反白显示；1 正常显示
  * @retval none
  * @note
  */ 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp, t, t1;
	uint8_t y0 = y;
	chr = chr - ' ';//得到偏移后的值				   
	for(t = 0; t < size; t++)
	{   
		if(size==12)temp = oled_asc2_1206[chr][t];	//调用1206字体
		else temp = oled_asc2_1608[chr][t];	//调用1608字体 	                          
		for(t1 = 0; t1 < 8; t1++)
		{
			if(temp & 0x80)OLED_DrawPoint(x, y, mode);
			else OLED_DrawPoint(x, y, !mode);
			temp <<= 1;
			y++;
			if((y-y0) == size)
			{
				y = y0;
				x++;
				break;
			}
		}  	 
   }          
}

/*
void OLED_ClearChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp, t, t1;
	uint8_t y0 = y;
	chr = chr - ' ';//得到偏移后的值				   
	for(t = 0; t < size; t++)
	{   
		if(size==12)temp = oled_asc2_1206[chr][t];	//调用1206字体
		else temp = oled_asc2_1608[chr][t];	//调用1608字体 	                          
		for(t1 = 0; t1 < 8; t1++)
		{
			if(temp & 0x80)OLED_DrawPoint(x, y, mode);
			else OLED_DrawPoint(x, y, !mode);
			temp <<= 1;
			y++;
			if((y-y0) == size)
			{
				y = y0;
				x++;
				break;
			}
		}  	 
   }          
}
*/
/**
  * @brief 显示字符串
	* @param x:（0~127）
	* @param y:（0~63 ）
  * @param *p:字符串起始地址
  * @retval none
  * @note 用16字体
  */
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p)
{
//#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSX 250
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
//        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
				if(y>MAX_CHAR_POSY){y=x=0;return;}
//				if(y>MAX_CHAR_POSY){y=x=0;break;}
        OLED_ShowChar(x,y,*p,16,1);	 
				if(x>=248)
				{x+=6;}
				else
        {x+=8;}
        p++;
    }  
}	   


/**
  * @brief m^n函数
	* @param m:基数
	* @param n:幂
  * @retval m^n
  * @note
  */ 
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  

/**
  * @brief 在指定位置显示数字
	* @param x:（0~127）
	* @param y:（0~63 ）
  * @param num:数值(0~4294967295);
  * @param len: 数字位数
	* @param size: 选择字体 16/12 
  * @retval none
  * @note
  */ 
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
}


//send as command
void Write_Register(unsigned char Data)
{
	unsigned char i;
	
	UGCS_0;
	UGA0_0;
	for (i=0; i<8; i++)
	{
		UGCK_0;                
		if (Data&0x80)
		{
			UGDI_1;
		}
		else
		{
			UGDI_0;
		}
		Data = Data << 1;
		UGCK_1;
	}
	UGA0_1;
	UGCS_1;	
}


//send data
void Write_Parameter(unsigned char Data)
{
	unsigned char i;
	
	UGCS_0;
	UGA0_1;
	for (i=0; i<8; i++)
	{
		UGCK_0;                
		if (Data&0x80)
		{
			UGDI_1;
		}
		else
		{
			UGDI_0;
		}
		Data = Data << 1;
		UGCK_1;
	}
	UGA0_1;
	UGCS_1;	
}

void Delay(unsigned int Data)
{
	unsigned char temp;
	for(; Data>0; Data--)
	{
		for(temp=0; temp<0xff; temp++);
	}
}

//******************************************************************************
//    函数说明：OLED屏初始化
//    入口数据：无
//    返回值：  无
//******************************************************************************
void Initial_ssd1325(void)
{		
	
	 	GPIO_InitTypeDef  GPIO_InitStructure;
	
 	/* 使能GPIOB,GPIOC，AFIO时钟 */
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
	
	/* PC13配置为推挽输出，用于控制OLED电源开关，注意PC13不支持5V耐压，速度只能配置到2MHz（详情参考规格书） */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//速度2MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//初始化
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//打开OLED电源
	
  /* 配置STM32为SWD调试模式，注意要先开启AFIO时钟 */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//SWD模式
	
	/* 配置PB3,PB4,PB5,PB6,PB7推挽输出 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);						//初始化
 	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	
	UGRST_0;	
	Delay(30);	
	UGRST_1;
	
	
	Write_Register(0xFD);           	// Unlock Command
	Write_Parameter(0x12);    		
	
	Write_Register(0xAE);           	// Display Off
	
	Write_Register(0xB3);            	// Set Clock 
	Write_Parameter(0x91);    		// 135 Frames/Sec
	
	Write_Register(0xCA);			// 1/64 Duty (0x0F~0x5F)			
	Write_Parameter(0x3F); 
	
	Write_Register(0xA2);			// Set Display Offset
	Write_Parameter(0x00); 
	
	Write_Register(0xA1);			// Set Mapping RAM Display Start Line (0x00~0x5F)
	Write_Parameter(0x00); 
	
	Write_Register(0xA0);			
	Write_Parameter(0x14); 			
	Write_Parameter(0x11);			// Set Column Address 0 Mapped to SEG0
								//     Disable Nibble Remap
								//     Horizontal Address Increment
								//     Scan from COM[N-1] to COM0
								//     Disable COM Split Odd Even
								//     Enable Dual COM Line Mode
	
	
	Write_Register(0xB5);			// Set GPIO off
	Write_Parameter(0x00);
	
	Write_Register(0xB1);			// Set Phase 1 as 5 Clocks & Phase 2 as 5 Clocks
	Write_Parameter(0x52);
	
	Write_Register(0xAB);			// Disable Embedded DC/DC Converter (0x00/0x01)
	Write_Parameter(0x01);
	
	Write_Register(0xB4);			// Enable External VSL
	Write_Parameter(0xA0);
	Write_Parameter(0xFD);
	
	Write_Register(0xC1);			// Set Brightness
	Write_Parameter(0x9F);	
	
	Write_Register(0xC7);			// Set Master Contrast Current
	Write_Parameter(0x0f);	
	
	Write_Register(0xB9);			// Set Gray Scale Table
	
	Write_Register(0xB1);			// Set Phase Lenth
	Write_Parameter(0xE2);
	
	Write_Register(0xD1);			// Set Enhance Driving Scheme Capability
	Write_Parameter(0x82);
	Write_Parameter(0x20);	
	
	Write_Register(0xBB);			// Set Pre-Charge Voltage Level
	Write_Parameter(0x1f);
	
	Write_Register(0xBE);			// Set High Voltage Level of COM Pin
	Write_Parameter(0x07);	
	
	Write_Register(0xA6);			// Normal Display Mode
	
	Write_Register(0xAF);			// Display On 	
}

//******************************************************************************
//    函数说明：OLED清屏显示
//    入口数据：无
//    返回值：  无
//******************************************************************************
void ClearLED (unsigned char color)
{
	unsigned char x,y;

	Set_Column_Address(0,63);
	Set_Row_Address(0,63);
        
	Write_Register(0x5C);    //开始写RAM命令
	
	for(y=0;y<64;y++)
	{                   
		for(x=0;x<128;x++)
		{    
			Write_Parameter(color | (color << 4));
		}
	}
}


//******************************************************************************
//    函数说明：OLED显示列的起始终止地址
//    入口数据：a  列的起始地址
//              b  列的终止地址
//    返回值：  无
//******************************************************************************
void Set_Column_Address(unsigned char a, unsigned char b)
{
	Write_Register(0x15);			        // Set Column Address
	Write_Parameter(0x1c+a);				//   Default => 0x00
	Write_Parameter(0x1c+b);				//   Default => 0x77
}

//******************************************************************************
//    函数说明：OLED显示行的起始终止地址
//    入口数据：a  行的起始地址
//              b  行的终止地址
//    返回值：  无
//******************************************************************************
void Set_Row_Address(unsigned char a, unsigned char b)
{
	Write_Register(0x75);			// Set Row Address
	Write_Parameter(a);				//   Default => 0x00
	Write_Parameter(b);				//   Default => 0x3F
}

//void Fill(void)
void OLED_Refresh_Gram(void)
{
	unsigned char x,y;
	
	Write_Register(0x15);   
	Write_Parameter(0x1c);    
	Write_Parameter(0x5b);   
	
	Write_Register(0x75);     
	Write_Parameter(0x00);       
	Write_Parameter(0x3f);
	
	Write_Register(0x5C);
	
	for(y=0;y<64;y++)
	{                   
		for(x=0;x<64;x++)
		{    
			Write_Parameter(OLED_GRAM[x*2][y]);
			Write_Parameter(OLED_GRAM[x*2+1][y]);
		}
	}
}

