/*
 * Copyright (C) 2010 The Paparazzi Team
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

/** \file mcu_periph/uart.h
 *  \brief arch independent UART (Universal Asynchronous Receiver/Transmitter) API
 *
 */

#ifndef MCU_PERIPH_UART_H
#define MCU_PERIPH_UART_H

#include "inc_types.h"
#include "uart_reg.h"
//#include "mcu_periph/uart_arch.h"
//#include "std.h"

#define USE_UART1 1
#define USE_UART2 1


#define UART_RX_BUFFER_SIZE 128
#define UART_TX_BUFFER_SIZE 128
#define UART_DEV_NAME_SIZE 16




/**
 * UART peripheral
 */
struct uart_periph {
  /** Receive buffer */
  uint8_t rx_buf[UART_RX_BUFFER_SIZE];
  uint16_t rx_insert_idx;
  uint16_t rx_extract_idx;
  /** Transmit buffer */
  uint8_t tx_buf[UART_TX_BUFFER_SIZE];
  uint16_t tx_insert_idx;
  uint16_t tx_extract_idx;
  uint8_t tx_running;
  /** UART Register */
  void* reg_addr;
  /** UART Dev (linux) */
  char dev[UART_DEV_NAME_SIZE];
  volatile uint16_t ore;    ///< overrun error counter
  volatile uint16_t ne_err; ///< noise error counter
  volatile uint16_t fe_err; ///< framing error counter
};

extern void usart1_isr(void);
extern void usart2_isr(void);





extern void uart_periph_init(struct uart_periph* p);
//extern void uart_periph_set_baudrate(struct uart_periph* p, uint32_t baud);
//extern void uart_periph_set_bits_stop_parity(struct uart_periph* p, uint8_t bits, uint8_t stop, uint8_t parity);
//extern void uart_periph_set_mode(struct uart_periph* p, bool_t tx_enabled, bool_t rx_enabled, bool_t hw_flow_control);
extern void uart_transmit(struct uart_periph* p, uint8_t data);
extern bool_t uart_check_free_space(struct uart_periph* p, uint8_t len);
uint8_t uart_getch(struct uart_periph* p);

/**
 * Check UART for available chars in receive buffer.
 * @return number of chars in the buffer
 */
static uint16_t uart_char_available(struct uart_periph* p) {
  int16_t available = p->rx_insert_idx - p->rx_extract_idx;
  if (available < 0)
    available += UART_RX_BUFFER_SIZE;
  return (uint16_t)available;
}




#if USE_UART1
extern struct uart_periph uart1;
//struct uart_periph uart1;
extern void uart1_init(void);

#define UART1Init() uart_periph_init(&uart1)
#define UART1CheckFreeSpace(_x) uart_check_free_space(&uart1, _x)
#define UART1Transmit(_x) uart_transmit(&uart1, _x)
#define UART1SendMessage() {}
#define UART1ChAvailable() uart_char_available(&uart1)
#define UART1Getch() uart_getch(&uart1)
#define UART1TxRunning uart1.tx_running
#define UART1SetBaudrate(_b) uart_periph_set_baudrate(&uart1, _b)
#define UART1SetBitsStopParity(_b, _s, _p) uart_periph_set_bits_stop_parity(&uart1, _b, _s, _p)

#endif // USE_UART1

#if USE_UART2
extern struct uart_periph uart2;
extern void uart2_init(void);

#define UART2Init() uart_periph_init(&uart2)
#define UART2CheckFreeSpace(_x) uart_check_free_space(&uart2, _x)
#define UART2Transmit(_x) uart_transmit(&uart2, _x)
#define UART2SendMessage() {}
#define UART2ChAvailable() uart_char_available(&uart2)
#define UART2Getch() uart_getch(&uart2)
#define UART2TxRunning uart2.tx_running
#define UART2SetBaudrate(_b) uart_periph_set_baudrate(&uart2, _b)
#define UART2SetBitsStopParity(_b, _s, _p) uart_periph_set_bits_stop_parity(&uart2, _b, _s, _p)

#endif // USE_UART2

#if USE_UART3
extern struct uart_periph uart3;
extern void uart3_init(void);

#define UART3Init() uart_periph_init(&uart3)
#define UART3CheckFreeSpace(_x) uart_check_free_space(&uart3, _x)
#define UART3Transmit(_x) uart_transmit(&uart3, _x)
#define UART3SendMessage() {}
#define UART3ChAvailable() uart_char_available(&uart3)
#define UART3Getch() uart_getch(&uart3)
#define UART3TxRunning uart3.tx_running
#define UART3SetBaudrate(_b) uart_periph_set_baudrate(&uart3, _b)
#define UART3SetBitsStopParity(_b, _s, _p) uart_periph_set_bits_stop_parity(&uart3, _b, _s, _p)

#endif // USE_UART3



#endif /* MCU_PERIPH_UART_H */
