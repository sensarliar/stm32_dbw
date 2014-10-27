/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*
void PPP_IRQHandler(void)
{
}
*/

/**
  * @brief  USART1�����ж�
  * @param  None
  * @retval None
  * @note
  *    ��Ч��һ֡�����Իس����н�β'\r'+'\n'
  */
void USART1_IRQHandler(void)
{
	uint8_t data;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//�����ж�
	{
		data = USART_ReceiveData(USART1);			//��ȡ���յ�������
		if((COM_RX_STA & 0x8000) == 0)				//����δ���
		{
			if(COM_RX_STA & 0x4000)							//���յ��˻س���
			{
				if(data != '\n') COM_RX_STA = 0;	//û���յ����м������մ���,���¿�ʼ
				else COM_RX_STA	|=	0x8000;				//���յ����м������������ 
			}
			else //��û�յ�0x0d
			{	
				if(data == '\r')	COM_RX_STA	|=	0x4000;//���յ��س���
				else
				{
					COM_RX_BUF[COM_RX_STA & 0x3fff]	=	data ;
					COM_RX_STA++;
					if(COM_RX_STA	>	(COM_RX_LEN - 1))	COM_RX_STA = 0;//�������
				}		 
			}
		} 
	}
}

/**
  * @brief  USART2�����ж�
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
	uint8_t Res;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	//�����ж�
	{
		Res = USART_ReceiveData(USART2);	//��ȡ���յ�������
		USART_SendData(USART2, Res);			//�����յ������ݷ��ͳ�ȥ  		 
	}
}

/**
  * @brief  USART3�����ж�
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
	uint8_t Res;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)	//�����ж�
	{
		Res = USART_ReceiveData(USART3);	//��ȡ���յ�������
		USART_SendData(USART3, Res);			//�����յ������ݷ��ͳ�ȥ  		 
	}
}

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
