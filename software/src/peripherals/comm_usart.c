/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2019 PCB Arts GmbH - Kevin Schick
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  _____    ____    ____
 *       \  /       |    \        /\            |
 *   ____/  |       |____/       /  \     __  __|__   ___
 *  |       |       |    \      /____\   /      |    |___
 *  |       \____   |____/     /      \  |      |     ___|
 *
 *         _    __   __   __     __         _    __   __
 *  \  /  /_\  |__| |  | |__|   |__| |__|  /_\  |__  |__
 *   \/  /   \ |    |__| | \    |    |  | /   \  __| |__
 *
 *
 *  PCB Arts GmbH
 *  Kurgartenstrasse 59
 *  90762 Fuerth
 *  GERMANY
 *
 *  www.pcb-arts.com
 *  mail(at)pcb-arts.com
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


//=================================
// included dependencies
#include "interrupt_priorities.h"
#include "peripherals/comm_usart.h"
#include "peripherals/comm_usart_isr.h"

#include <stm32f3xx.h>

//=================================
// definitions

#define COMM_USART_FREQ 115200

static void (*receiver_cb)(const char character);
static void (*send_cb)();

void comm_usart_init() {
	//enable peripheral clock
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);

	//disable UART module
	CLEAR_BIT(USART2->CR1, USART_CR1_UE);

	CLEAR_BIT(USART2->CR1,
		//00: 1 Start bit, 8 data bits, n stop bits
		USART_CR1_M |

		//Parity control disabled
		USART_CR1_PCE |

		//Even parity
		USART_CR1_PS |

		//Oversampling by 16
		USART_CR1_OVER8);

	CLEAR_BIT(USART2->CR2,
		//1 stop bit
		USART_CR2_STOP |

		//LIN mode disabled
		USART_CR2_LINEN |

		//CK pin disabled
		USART_CR2_CLKEN);

	MODIFY_REG(USART2->CR3,
		////////// clear bits //////////
		//RTS hardware flow control disabled
		USART_CR3_RTSE |

		//CTS hardware flow control disabled
		USART_CR3_CTSE |

		//Three sample bit method
		USART_CR3_ONEBIT |

		//Smartcard Mode disabled
		USART_CR3_SCEN |

		//Half duplex mode is not selected
		USART_CR3_HDSEL |

		//IrDA disabled
		USART_CR3_IREN,

		////////// set bits //////////
		USART_CR3_EIE);

	//set USART baud rate
	USART2->BRR = (SystemCoreClock / 2) / COMM_USART_FREQ;

	SET_BIT(USART2->CR1,
		//Enable the UART Parity Error and Data Register not empty Interrupts
		USART_CR1_RXNEIE |

		//enable UART module
		USART_CR1_UE |

		//start receiver
		USART_CR1_RE);

	//set interrupt priority
	NVIC_SetPriority(USART2_IRQn, NVIC_PRIO_COMMAND_USART);

	//enable interrupt
	NVIC_EnableIRQ(USART2_IRQn);
}

void comm_usart_rxne_isr() {
	//get character from peripheral register
	const char character = USART2->RDR;

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
	USART2->TDR = buf;
}

void comm_usart_enable_transmitter() {
	SET_BIT(USART2->CR1,
		//enable transmitter
		USART_CR1_TE |

		//enable transmit interrupt
		USART_CR1_TXEIE);
}

void comm_usart_disable_transmitter() {
	CLEAR_BIT(USART2->CR1,
		//disable transmitter
		USART_CR1_TE |

		//disable transmit empty interrupt
		USART_CR1_TXEIE |

		//disable transmission complete interrupt
		USART_CR1_TCIE);
}

void comm_usart_filalize_xfer() {
	MODIFY_REG(USART2->CR1,
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
