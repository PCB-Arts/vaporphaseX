/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2019 - PCB Arts GmbH - Kevin Schick
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
#include "peripherals/i2c.h"

#include <stm32f3xx.h>

//=================================
// defines

#define I2C_RESET_DALAY_MS 50

static volatile struct I2C_set_t* I2C_set;
static volatile uint8_t set_index;

volatile uint8_t I2C_reset_ms;
volatile uint8_t I2C_watchdog;

static void init_DMA1_CH6();
static void init_DMA1_CH7();

void I2C_millis_event() {
	if (I2C_reset_ms > 0) I2C_reset_ms--;
	if (I2C_watchdog > 0) I2C_watchdog--;
}

void I2C_init(volatile struct I2C_set_t* set) {
	//enable peripheral clock
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);

	I2C_reconfigure();

	I2C_set = set;
	set_index = 0;
}

void I2C_reconfigure() {
	//reset I2C peripheral
	SET_BIT(RCC->APB1RSTR, RCC_APB1RSTR_I2C1RST);
	CLEAR_BIT(RCC->APB1RSTR, RCC_APB1RSTR_I2C1RST);

	//timing:
	// mode: standard
	// frequency: 100 kHz
	// rise time: 410 ns
	// fall time: 15 ns
	// analog filter: enabled
	// digital filter: 0
	I2C1->TIMINGR = 0x20F05F81;

	init_DMA1_CH6();
	init_DMA1_CH7();

	//enable interrupt lines
	NVIC_EnableIRQ(I2C1_EV_IRQn);
	NVIC_EnableIRQ(I2C1_ER_IRQn);
}

static void init_DMA1_CH6() {
	//disable DMA channel
	CLEAR_BIT(DMA1_Channel6->CCR, DMA_CCR_EN);

	SET_BIT(DMA1_Channel6->CCR,
		//Memory increment mode enabled
		DMA_CCR_MINC |

		//Read from memory
		DMA_CCR_DIR);

	//Peripheral address
	DMA1_Channel6->CPAR = (uint32_t)&I2C1->TXDR;
}

static void init_DMA1_CH7() {
	//disable DMA channel
	CLEAR_BIT(DMA1_Channel7->CCR, DMA_CCR_EN);

	SET_BIT(DMA1_Channel7->CCR,
		//Memory increment mode enabled
		DMA_CCR_MINC);

	//Peripheral address
	DMA1_Channel7->CPAR = (uint32_t)&I2C1->RXDR;
}

void I2C1_EV_IRQHandler() {
	//check for all interrupts
	if (I2C1->ISR != 0x00){
		I2C_set->devices[set_index].isr();
	}
}

void I2C1_ER_IRQHandler() {
	I2C_reset();
}

void I2C_worker() {
	if (I2C_reset_ms != 0)
		return;

	if (!I2C_set)
		return;

	//check max index
	if (set_index >= I2C_set->count)
		set_index = 0;

	//execute process function
	if (I2C_set->devices[set_index].process())
		set_index++;
}

void I2C_reset() {
	//disable interrupt lines
	NVIC_DisableIRQ(I2C1_EV_IRQn);
	NVIC_DisableIRQ(I2C1_ER_IRQn);

	//set reset timeout
	I2C_reset_ms = I2C_RESET_DALAY_MS;

	//reset I2C modules/devices
	for (uint_fast8_t i = 0; i < I2C_set->count; ++i) {
		I2C_set->devices[i].reset();
	}

	I2C_reconfigure();
}
