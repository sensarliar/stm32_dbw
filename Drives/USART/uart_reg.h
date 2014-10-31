/** @addtogroup usart_defines

@author @htmlonly &copy; @endhtmlonly 2009 Uwe Hermann <uwe@hermann-uwe.de>

*/

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/**@{*/

/* THIS FILE SHOULD NOT BE INCLUDED DIRECTLY, BUT ONLY VIA USART.H
The order of header inclusion is important. usart.h includes the device
specific memorymap.h header before including this header file.*/


#ifndef LIBOPENCM3_USART_COMMON_F124_H
#define LIBOPENCM3_USART_COMMON_F124_H

/* Generic memory-mapped I/O accessor functions */
#define MMIO8(addr)		(*(volatile uint8_t *)(addr))
#define MMIO16(addr)		(*(volatile uint16_t *)(addr))
#define MMIO32(addr)		(*(volatile uint32_t *)(addr))
#define MMIO64(addr)		(*(volatile uint64_t *)(addr))

//#include <libopencm3/stm32/common/usart_common_all.h>
/* --- USART registers ----------------------------------------------------- */

/* Status register (USARTx_SR) */
#define USART_SR(usart_base)		MMIO32(usart_base + 0x00)
#define USART1_SR			USART_SR(USART1_BASE)
#define USART2_SR			USART_SR(USART2_BASE)
#define USART3_SR			USART_SR(USART3_BASE)
#define UART4_SR			USART_SR(UART4_BASE)
#define UART5_SR			USART_SR(UART5_BASE)

/* Data register (USARTx_DR) */
#define USART_DR(usart_base)		MMIO32(usart_base + 0x04)
#define USART1_DR			USART_DR(USART1_BASE)
#define USART2_DR			USART_DR(USART2_BASE)
#define USART3_DR			USART_DR(USART3_BASE)
#define UART4_DR			USART_DR(UART4_BASE)
#define UART5_DR			USART_DR(UART5_BASE)

/* Baud rate register (USARTx_BRR) */
#define USART_BRR(usart_base)		MMIO32(usart_base + 0x08)
#define USART1_BRR			USART_BRR(USART1_BASE)
#define USART2_BRR			USART_BRR(USART2_BASE)
#define USART3_BRR			USART_BRR(USART3_BASE)
#define UART4_BRR			USART_BRR(UART4_BASE)
#define UART5_BRR			USART_BRR(UART5_BASE)

/* Control register 1 (USARTx_CR1) */
#define USART_CR1(usart_base)		MMIO32(usart_base + 0x0c)
#define USART1_CR1			USART_CR1(USART1_BASE)
#define USART2_CR1			USART_CR1(USART2_BASE)
#define USART3_CR1			USART_CR1(USART3_BASE)
#define UART4_CR1			USART_CR1(UART4_BASE)
#define UART5_CR1			USART_CR1(UART5_BASE)

/* Control register 2 (USARTx_CR2) */
#define USART_CR2(usart_base)		MMIO32(usart_base + 0x10)
#define USART1_CR2			USART_CR2(USART1_BASE)
#define USART2_CR2			USART_CR2(USART2_BASE)
#define USART3_CR2			USART_CR2(USART3_BASE)
#define UART4_CR2			USART_CR2(UART4_BASE)
#define UART5_CR2			USART_CR2(UART5_BASE)

/* Control register 3 (USARTx_CR3) */
#define USART_CR3(usart_base)		MMIO32(usart_base + 0x14)
#define USART1_CR3			USART_CR3(USART1_BASE)
#define USART2_CR3			USART_CR3(USART2_BASE)
#define USART3_CR3			USART_CR3(USART3_BASE)
#define UART4_CR3			USART_CR3(UART4_BASE)
#define UART5_CR3			USART_CR3(UART5_BASE)

/* Guard time and prescaler register (USARTx_GTPR) */
#define USART_GTPR(usart_base)		MMIO32(usart_base + 0x18)
#define USART1_GTPR			USART_GTPR(USART1_BASE)
#define USART2_GTPR			USART_GTPR(USART2_BASE)
#define USART3_GTPR			USART_GTPR(USART3_BASE)
#define UART4_GTPR			USART_GTPR(UART4_BASE)
#define UART5_GTPR			USART_GTPR(UART5_BASE)

#endif 
