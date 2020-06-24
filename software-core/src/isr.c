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
#include "modules/heater.h"
#include "modules/lid.h"
#include "modules/lid_isr.h"
#include "modules/lift.h"
#include "modules/lift_isr.h"
#include "modules/axis_isr.h"
#include "peripherals/comm_usart_isr.h"

#include <stm32f3xx.h>

//=================================
// definitions

void USART2_EXTI26_IRQHandler() {
	//receiver not empty interrupt
	if (READ_BIT(USART2->CR1, USART_CR1_RXNEIE) &&
			READ_BIT(USART2->ISR, USART_ISR_RXNE)){

		comm_usart_rxne_isr();
	}

	//transmit empty interrupt
	if (READ_BIT(USART2->CR1, USART_CR1_TXEIE) &&
		READ_BIT(USART2->ISR, USART_ISR_TXE)){

		comm_usart_txe_isr();
	}

	//transmission complete interrupt
	if (READ_BIT(USART2->CR1, USART_CR1_TCIE) &&
		READ_BIT(USART2->ISR, USART_ISR_TC)){

		comm_usart_tc_isr();
	}
}

void EXTI9_5_IRQHandler() {
	//check lid encoders
	if (READ_BIT(EXTI->PR, EXTI_PR_PR6 | EXTI_PR_PR7)) {
		lid_encoder_isr();

		//clear interrupt flags
		EXTI->PR = (EXTI_PR_PR6 | EXTI_PR_PR7);
	}

	//check lift encoders
	if (READ_BIT(EXTI->PR, EXTI_PR_PR8 | EXTI_PR_PR9)) {
		lift_encoder_isr();

		//clear interrupt flags
		EXTI->PR = (EXTI_PR_PR8 | EXTI_PR_PR9);
	}
}


void EXTI0_IRQHandler() {
	//check limit switch lift top
	if (READ_BIT(EXTI->PR, EXTI_PR_PR0)) {
		axis_limsw_fwd_isr(&lift_axis);

		//clear interrupt flags
		SET_BIT(EXTI->PR, EXTI_PR_PR0);
	}
}

void EXTI1_IRQHandler() {
	//check limit switch lift bottom
	if (READ_BIT(EXTI->PR, EXTI_PR_PR1)) {
		axis_limsw_bwd_isr(&lift_axis);

		//clear interrupt flags
		SET_BIT(EXTI->PR, EXTI_PR_PR1);
	}
}

void EXTI2_TSC_IRQHandler() {
	//check limit switch lid top
	if (READ_BIT(EXTI->PR, EXTI_PR_PR2)) {
		axis_limsw_fwd_isr(&lid_axis);

		//clear interrupt flags
		SET_BIT(EXTI->PR, EXTI_PR_PR2);
	}
}

void EXTI3_IRQHandler() {
	//check limit switch lid bottom
	if (READ_BIT(EXTI->PR, EXTI_PR_PR3)) {
		axis_limsw_bwd_isr(&lid_axis);

		//clear interrupt flags
		SET_BIT(EXTI->PR, EXTI_PR_PR3);
	}
}

void EXTI4_IRQHandler() {
	//check heater over temperature error
	if (READ_BIT(EXTI->PR, EXTI_PR_PR4)) {
		heater_overtemp();

		//clear interrupt flags
		SET_BIT(EXTI->PR, EXTI_PR_PR4);
	}
}
