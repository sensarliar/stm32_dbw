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
 
 #define USE_UART1 1
 
 
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

/* by default enable UART Tx and Rx */
#ifndef USE_UART2_TX
#define USE_UART2_TX TRUE
#endif
#ifndef USE_UART2_RX
#define USE_UART2_RX TRUE
#endif

#ifndef UART2_HW_FLOW_CONTROL
#define UART2_HW_FLOW_CONTROL FALSE
#endif

#ifndef UART2_BITS
#define UART2_BITS UBITS_8
#endif

#ifndef UART2_STOP
#define UART2_STOP USTOP_1
#endif

#ifndef UART2_PARITY
#define UART2_PARITY UPARITY_NO
#endif

void uart2_init( void ) {

  uart_periph_init(&uart2);
  uart2.reg_addr = (void *)USART2;

  /* init RCC and GPIOs */
  rcc_periph_clock_enable(RCC_USART2);

#if USE_UART2_TX
  gpio_setup_pin_af(UART2_GPIO_PORT_TX, UART2_GPIO_TX, UART2_GPIO_AF, TRUE);
#endif
#if USE_UART2_RX
  gpio_setup_pin_af(UART2_GPIO_PORT_RX, UART2_GPIO_RX, UART2_GPIO_AF, FALSE);
#endif

  /* Enable USART interrupts in the interrupt controller */
  usart_enable_irq(NVIC_USART2_IRQ);

#if UART2_HW_FLOW_CONTROL && defined(STM32F4)
#warning "USING UART2 FLOW CONTROL. Make sure to pull down CTS if you are not connecting any flow-control-capable hardware."
  /* setup CTS and RTS pins */
  gpio_setup_pin_af(UART2_GPIO_PORT_CTS, UART2_GPIO_CTS, UART2_GPIO_AF, FALSE);
  gpio_setup_pin_af(UART2_GPIO_PORT_RTS, UART2_GPIO_RTS, UART2_GPIO_AF, TRUE);
#endif

  /* Configure USART Tx,Rx, and hardware flow control*/
  uart_periph_set_mode(&uart2, USE_UART2_TX, USE_UART2_RX, UART2_HW_FLOW_CONTROL);

  /* Configure USART */
  uart_periph_set_bits_stop_parity(&uart2, UART2_BITS, UART2_STOP, UART2_PARITY);
  uart_periph_set_baudrate(&uart2, UART2_BAUD);
}

void usart2_isr(void) { usart_isr(&uart2); }

#endif /* USE_UART2 */


#if USE_UART3

/* by default enable UART Tx and Rx */
#ifndef USE_UART3_TX
#define USE_UART3_TX TRUE
#endif
#ifndef USE_UART3_RX
#define USE_UART3_RX TRUE
#endif

#ifndef UART3_HW_FLOW_CONTROL
#define UART3_HW_FLOW_CONTROL FALSE
#endif

#ifndef UART3_BITS
#define UART3_BITS UBITS_8
#endif

#ifndef UART3_STOP
#define UART3_STOP USTOP_1
#endif

#ifndef UART3_PARITY
#define UART3_PARITY UPARITY_NO
#endif

void uart3_init( void ) {

  uart_periph_init(&uart3);
  uart3.reg_addr = (void *)USART3;

  /* init RCC */
  rcc_periph_clock_enable(RCC_USART3);

#if USE_UART3_TX
  gpio_setup_pin_af(UART3_GPIO_PORT_TX, UART3_GPIO_TX, UART3_GPIO_AF, TRUE);
#endif
#if USE_UART3_RX
  gpio_setup_pin_af(UART3_GPIO_PORT_RX, UART3_GPIO_RX, UART3_GPIO_AF, FALSE);
#endif

  /* Enable USART interrupts in the interrupt controller */
  usart_enable_irq(NVIC_USART3_IRQ);

#if UART3_HW_FLOW_CONTROL && defined(STM32F4)
#warning "USING UART3 FLOW CONTROL. Make sure to pull down CTS if you are not connecting any flow-control-capable hardware."
  /* setup CTS and RTS pins */
  gpio_setup_pin_af(UART3_GPIO_PORT_CTS, UART3_GPIO_CTS, UART3_GPIO_AF, FALSE);
  gpio_setup_pin_af(UART3_GPIO_PORT_RTS, UART3_GPIO_RTS, UART3_GPIO_AF, TRUE);
#endif

  /* Configure USART Tx,Rx, and hardware flow control*/
  uart_periph_set_mode(&uart3, USE_UART3_TX, USE_UART3_RX, UART3_HW_FLOW_CONTROL);

  /* Configure USART */
  uart_periph_set_bits_stop_parity(&uart3, UART3_BITS, UART3_STOP, UART3_PARITY);
  uart_periph_set_baudrate(&uart3, UART3_BAUD);
}

void usart3_isr(void) { usart_isr(&uart3); }

#endif /* USE_UART3 */
