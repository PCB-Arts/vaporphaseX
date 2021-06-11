/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2021 - PCB Arts GmbH
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


//=================================
// included dependencies
#include "interrupt_priorities.h"
#include "peripherals/comm_usart.h"
#include "peripherals/comm_usart_isr.h"
#include "conf.h"

#include <stm32f3xx.h>

//=================================
// definitions

#define COMM_USART_FREQ 115200

static void (*receiver_cb)(const char character);
static void (*send_cb)();

USART_TypeDef *cli_usart = USART3;

void comm_usart_init() {
	// base uart configuration is configured using MX

	//disable UART module
	CLEAR_BIT(cli_usart->CR1, USART_CR1_UE);


	SET_BIT(cli_usart->CR1,
		//Enable the UART Parity Error and Data Register not empty Interrupts
		USART_CR1_RXNEIE |

		//enable UART module
		USART_CR1_UE |

		//start receiver
		USART_CR1_RE);
}

void comm_usart_rxne_isr() {
	//get character from peripheral register
	const char character = cli_usart->RDR;

	//check and call receiver callback
	if (receiver_cb)
		receiver_cb(character);
}

void comm_usart_txe_isr() {
	//check and call send callback
	if (send_cb)
		send_cb();
}

void comm_usart_tc_isr() {
	//disable transmitter
	comm_usart_disable_transmitter();
}

void comm_usart_send(volatile char buf) {
	//copy character into TX peripheral register
	cli_usart->TDR = buf;
}

void comm_usart_enable_transmitter() {
	SET_BIT(cli_usart->CR1,
		//enable transmitter
		USART_CR1_TE |

		//enable transmit interrupt
		USART_CR1_TXEIE);
}

void comm_usart_disable_transmitter() {
	CLEAR_BIT(cli_usart->CR1,
		//disable transmitter
		USART_CR1_TE |

		//disable transmit empty interrupt
		USART_CR1_TXEIE |

		//disable transmission complete interrupt
		USART_CR1_TCIE);
}

void comm_usart_filalize_xfer() {
	MODIFY_REG(cli_usart->CR1,
		////////// clear bits //////////
		//disable transmit empty interrupt
		USART_CR1_TXEIE,

		////////// set bits //////////
		//enable transmission complete interrupt
		USART_CR1_TCIE);
}

void comm_usart_set_receiver_cb(void (*cb)(const char character)) {
	//set receiver callback function
	receiver_cb = cb;
}

void comm_usart_set_send_cb(void (*cb)()) {
	//set send callback function
	send_cb = cb;
}
