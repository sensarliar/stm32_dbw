/**
 * @file arch/stm32/mcu_periph/uart_arch.c
 * @ingroup stm32_arch
 *
 * Handling of UART hardware for STM32.
 */
 
 #include "uart_arch.h"
 #include "inc_types.h"
 #include "uart_reg.h"
// #include "stm32f10x.h"
#include "usart.h"
 
// #define USE_UART1 1
// #define USE_UART2 1
 
 
 void usart_send(uint32_t usart, uint8_t data)
{
	USART_DR(usart) = data;
}

uint8_t usart_recv(uint32_t usart)
{
	/* Receive data. */
	return USART_DR(usart);
}


 
void uart_transmit(struct uart_periph* p, uint8_t data ) {

  uint16_t temp = (p->tx_insert_idx + 1) % UART_TX_BUFFER_SIZE;

  if (temp == p->tx_extract_idx)
    return;                          // no room

  USART_CR1((uint32_t)p->reg_addr) &= ~USART_CR1_TXEIE; // Disable TX interrupt

  // check if in process of sending data
  if (p->tx_running) { // yes, add to queue
    p->tx_buf[p->tx_insert_idx] = data;
    p->tx_insert_idx = temp;
  }
  else { // no, set running flag and write to output register
    p->tx_running = TRUE;
    usart_send((uint32_t)p->reg_addr, data);
//				USART_SendData(USART1, COM_RX_BUF[i]);//串口发送一个数据		
//		USART_SendData((uint32_t)p->reg_addr, data);//串口发送一个数据
  }

  USART_CR1((uint32_t)p->reg_addr) |= USART_CR1_TXEIE; // Enable TX interrupt

}

static void usart_isr(struct uart_periph* p) {

  if (((USART_CR1((uint32_t)p->reg_addr) & USART_CR1_TXEIE) != 0) &&
      ((USART_SR((uint32_t)p->reg_addr) & USART_SR_TXE) != 0)) {
    // check if more data to send
    if (p->tx_insert_idx != p->tx_extract_idx) {
      usart_send((uint32_t)p->reg_addr,p->tx_buf[p->tx_extract_idx]);
      p->tx_extract_idx++;
      p->tx_extract_idx %= UART_TX_BUFFER_SIZE;
    }
    else {
      p->tx_running = FALSE;   // clear running flag
      USART_CR1((uint32_t)p->reg_addr) &= ~USART_CR1_TXEIE; // Disable TX interrupt
    }
  }

  if (((USART_CR1((uint32_t)p->reg_addr) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR((uint32_t)p->reg_addr) & USART_SR_RXNE) != 0) &&
      ((USART_SR((uint32_t)p->reg_addr) & USART_SR_ORE) == 0) &&
      ((USART_SR((uint32_t)p->reg_addr) & USART_SR_NE) == 0) &&
      ((USART_SR((uint32_t)p->reg_addr) & USART_SR_FE) == 0)) {
    uint16_t temp = (p->rx_insert_idx + 1) % UART_RX_BUFFER_SIZE;;
    p->rx_buf[p->rx_insert_idx] = usart_recv((uint32_t)p->reg_addr);
    // check for more room in queue
    if (temp != p->rx_extract_idx)
      p->rx_insert_idx = temp; // update insert index
  }
  else {
    /* ORE, NE or FE error - read USART_DR reg and log the error */
    if (((USART_CR1((uint32_t)p->reg_addr) & USART_CR1_RXNEIE) != 0) &&
        ((USART_SR((uint32_t)p->reg_addr) & USART_SR_ORE) != 0)) {
      usart_recv((uint32_t)p->reg_addr);
      p->ore++;
    }
    if (((USART_CR1((uint32_t)p->reg_addr) & USART_CR1_RXNEIE) != 0) &&
        ((USART_SR((uint32_t)p->reg_addr) & USART_SR_NE) != 0)) {
      usart_recv((uint32_t)p->reg_addr);
      p->ne_err++;
    }
    if (((USART_CR1((uint32_t)p->reg_addr) & USART_CR1_RXNEIE) != 0) &&
        ((USART_SR((uint32_t)p->reg_addr) & USART_SR_FE) != 0)) {
      usart_recv((uint32_t)p->reg_addr);
      p->fe_err++;
    }
  }
}

void uart_periph_init(struct uart_periph* p) {
  p->rx_insert_idx = 0;
  p->rx_extract_idx = 0;
  p->tx_insert_idx = 0;
  p->tx_extract_idx = 0;
  p->tx_running = FALSE;
  p->ore = 0;
  p->ne_err = 0;
  p->fe_err = 0;


}



bool_t uart_check_free_space(struct uart_periph* p, uint8_t len) {
  int16_t space = p->tx_extract_idx - p->tx_insert_idx;
  if (space <= 0)
    space += UART_TX_BUFFER_SIZE;
  return (uint16_t)(space - 1) >= len;
}

uint8_t uart_getch(struct uart_periph* p) {
  uint8_t ret = p->rx_buf[p->rx_extract_idx];
  p->rx_extract_idx = (p->rx_extract_idx + 1) % UART_RX_BUFFER_SIZE;
  return ret;
}





#if USE_UART1
struct uart_periph uart1;

void uart1_init( void ) {
	
  uart_periph_init(&uart1);
  uart1.reg_addr = (void *)USART1;
//	COM_Init(COM1, 115200);//串口初始化
	COM_Init(COM1, 9600);//串口初始化
	
}

void usart1_isr(void) { usart_isr(&uart1); }

#endif /* USE_UART1 */


#if USE_UART2

struct uart_periph uart2;

void uart2_init( void ) {

  uart_periph_init(&uart2);
  uart2.reg_addr = (void *)USART2;
	//COM_Init(COM2, 115200);//串口初始化
	COM_Init(COM2, 9600);
}

void usart2_isr(void) { usart_isr(&uart2); }

#endif /* USE_UART2 */


#if USE_UART3

struct uart_periph uart3;

void uart3_init( void ) {

  uart_periph_init(&uart3);
  uart3.reg_addr = (void *)USART3;
	COM_Init(COM3, 115200);//串口初始化
}



void usart3_isr(void) { usart_isr(&uart3); }

#endif /* USE_UART3 */
