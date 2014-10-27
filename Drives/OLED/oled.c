/*******************************************************************************
 * @name    : OLEDģ��ײ�����
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
#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"

/**
  * OLED���Դ��Ÿ�ʽ
  * -------------------
  * [0]0 1 2 3 ... 127	
  * [1]0 1 2 3 ... 127	
  * [2]0 1 2 3 ... 127	
  * [3]0 1 2 3 ... 127	
  * [4]0 1 2 3 ... 127	
  * [5]0 1 2 3 ... 127	
  * [6]0 1 2 3 ... 127	
  * [7]0 1 2 3 ... 127
	*/
uint8_t OLED_GRAM[128][8];	 

/**
  * @brief �����Դ浽OLED
  * @param none
  * @retval none
  * @note 
  */
void OLED_Refresh_Gram(void)
{
	uint8_t page, col;		    
	for(page=0; page<8; page++)  
	{  
		OLED_WR_Byte (0xb0+page,OLED_CMD); //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(col=0; col<128; col++)
		  OLED_WR_Byte(OLED_GRAM[col][page],OLED_DATA); 
	}
}

/**
  * @brief ͨ��4��SPI��SSD1306д��һ���ֽڡ�
  * @param dat: Ҫд�������/����
  * @param DC:  ����/�����־ 1,��ʾ����; 0,��ʾ����;
  * @retval none
  * @note MSB -> LSB����˳��
  */
void OLED_WR_Byte(uint8_t dat,uint8_t DC)
{	
	uint8_t i;
	
	OLED_DC = DC;	
	OLED_CS = 0;
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK = 0;
		if(dat&0x80)
		   OLED_SDIN = 1;
		else 
		   OLED_SDIN = 0;
		OLED_SCLK = 1;
		dat<<=1;   
	}				 		  
	OLED_CS = 1;
	OLED_DC = 1;   	  
}
	  	  
/**
  * @brief ����OLED��ʾ
  * @param none
  * @retval none
  * @note �����ڲ���ѹ��·��charge pump��
  */
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //charge pump setting����
	OLED_WR_Byte(0X14,OLED_CMD);  //charge pump on�������ڲ���ѹ
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/**
  * @brief �ر�OLED��ʾ
  * @param none
  * @retval none
  * @note �ر��ڲ���ѹ��·��charge pump��
  */   
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //charge pump setting����
	OLED_WR_Byte(0X10,OLED_CMD);  //charge pump off���ر��ڲ���ѹ
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 

/**
  * @brief �����Ļ��ʾ
  * @param none
  * @retval none
  * @note ����Դ棬�������ض��ر�
  */ 
void OLED_Clear(void)  
{  
	uint8_t page, col;  
	for(page=0; page<8; page++)
	{
	  for(col=0; col<128; col++)
	    OLED_GRAM[col][page]=0x00;
	}
	OLED_Refresh_Gram();//������ʾ
}

/**
  * @brief ����
	* @param x: X�����꣨0~127��
	* @param y: Y�����꣨0~63 ��
	* @param dot: 1 ��䣻0 ���
  * @retval none
  * @note
  */ 
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t dot)
{
	uint8_t page, bit, data = 0;
	if(x>127 || y>63) return;//������Χ��.
	page = y / 8;
	bit = y % 8;
	data = 1 << bit;
	if(dot)OLED_GRAM[x][page] |= data;
	else OLED_GRAM[x][page] &= ~data;
}

/**
  * @brief x1,y1,x2,y2 �������ĶԽ�����
	* @param (x1,y1): ���������ʼ����
	* @param (x2,y2): ��������������
	* @param dot: 1 ��䣻0 ���
  * @retval none
  * @note ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 
  */ 
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}

/**
  * @brief ��ָ��λ����ʾһ���ַ�
	* @param x:��0~127��
	* @param y:��0~63 ��
	* @param size: ѡ������ 16/12 
  * @param mode: 0 ������ʾ��1 ������ʾ
  * @retval none
  * @note
  */ 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp, t, t1;
	uint8_t y0 = y;
	chr = chr - ' ';//�õ�ƫ�ƺ��ֵ				   
	for(t = 0; t < size; t++)
	{   
		if(size==12)temp = oled_asc2_1206[chr][t];	//����1206����
		else temp = oled_asc2_1608[chr][t];	//����1608���� 	                          
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

/**
  * @brief m^n����
	* @param m:����
	* @param n:��
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
  * @brief ��ָ��λ����ʾ����
	* @param x:��0~127��
	* @param y:��0~63 ��
  * @param num:��ֵ(0~4294967295);
  * @param len: ����λ��
	* @param size: ѡ������ 16/12 
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

/**
  * @brief ��ʾ����,��λΪ0,������ʾ
	* @param x:��0~127��
	* @param y:��0~63 ��
  * @param num:��ֵ(0~4294967295);
  * @param len: ����λ��
	* @param size: ѡ������ 16/12 
  * @param mode:
  * [7]:0,�����;1,���0.
  * [6:3]:����
  * [2:1]:00,ʮ���� 01,ʮ������ 
  * [0]:0,������ʾ;1,������ʾ
  * @retval none
  * @note
  */ 
void OLED_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;
	uint8_t system=0;
	
	system=(mode>>1)&0x03;
	
	for(t=0;t<len;t++)
	{
		if(system==1)//16����
		{
			temp=(num>>(4*(len-t-1)))&0x0f;
		}
		else//10����
		{
			temp=(num/oled_pow(10,len-t-1))%10;
		}
		
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0x80)OLED_ShowChar(x+(size/2)*t,y,'0',size,mode&0x01);  
				else OLED_ShowChar(x+(size/2)*t,y,' ',size,mode&0x01);  
 				continue;
			}else enshow=1; 
		 	 
		}
		if(system==1&&temp>=10)
			OLED_ShowChar(x+(size/2)*t,y,temp-10+'A',size,mode&0x01);
		else
			OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0x01); 
	}
}

/**
  * @brief ��ʾ�ַ���
	* @param x:��0~127��
	* @param y:��0~63 ��
  * @param *p:�ַ�����ʼ��ַ
  * @retval none
  * @note ��16����
  */
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);	 
        x+=8;
        p++;
    }  
}	   

/**
  * @brief  SSD1306��ʼ��	
  * @param  None
  * @retval None
	* @note  
  *    4��SPI���߷�ʽ��
  *    CS   -> PB3 ��������JTAG�� JTDO��
  *    RST  -> PB4 ��������JTAG�� JNTRST��
  *    D/C# -> PB5
  *    SCLK -> PB6
  *    SDIN -> PB7
  *    - OLEDģ���Դ�ɿأ����ƹܽ���PC13
  *    - PB3��PB4������JTAG��JTDO��JNTRST�����STM32�����ó�SWDģʽ
  */
void OLED_Init(void)
{ 	
 
 	GPIO_InitTypeDef  GPIO_InitStructure;
	
 	/* ʹ��GPIOB,GPIOC��AFIOʱ�� */
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
	
	/* PC13����Ϊ������������ڿ���OLED��Դ���أ�ע��PC13��֧��5V��ѹ���ٶ�ֻ�����õ�2MHz������ο�����飩 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//�ٶ�2MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//��ʼ��
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//��OLED��Դ
	
  /* ����STM32ΪSWD����ģʽ��ע��Ҫ�ȿ���AFIOʱ�� */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//SWDģʽ
	
	/* ����PB3,PB4,PB5,PB6,PB7������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);						//��ʼ��
 	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
  							  
	OLED_RST = 0;
	delay_ms(100);
	OLED_RST = 1; 
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
	OLED_WR_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
	OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
	OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
	OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00:ˮƽѰַģʽ; 01:��ֱѰַģʽ; 10:ҳѰַģʽ;Ĭ��10;
	OLED_WR_Byte(0xA1,OLED_CMD); //SEG�ض�������,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC8,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����
		 
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 
	OLED_Clear();
}

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/

