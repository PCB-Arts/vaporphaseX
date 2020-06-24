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
#include "main.h"
#include "ui/cli.h"
#include "ui/cli_commands.h"
#include <ui/lcd.h>
#include "system/core_clock.h"
#include "system/systime.h"
#include "interrupt_priorities.h"
#include "peripherals/spi.h"
#include "peripherals/gpio.h"
#include "peripherals/comm_usart.h"
#include "peripherals/exti.h"
#include "peripherals/pwm.h"
#include "modules/axis.h"
#include "modules/lid.h"
#include "modules/lift.h"
#include "modules/regulator.h"
#include "modules/temp_sensors.h"
#include "modules/heater.h"
#include "modules/fan.h"
#include "modules/fans.h"
#include "modules/sequence.h"
#include "modules/max31856/MAX31856.h"
#include "modules/ili9341/ili9341_core.h"
#include "hardware_declarations.h"

#include <stm32f3xx.h>

//=================================
// definitions

#define BROADCASTER_DELAY_MS 250
static uint16_t broadcaster_delay;

static void init_system();
static void init_peripherals();
static void init_modules();


int main(void) {
	init_system();
	init_peripherals();
	init_modules();

	//init cooler fans
	fan_set_level(&hydro_cooler, 60);
	fan_set_level(&quick_cooler, 0);

	//enable LED lighting inside the soldering chamber
	io_write_pin(LIGHTING_ENABLE_PORT, LIGHTING_ENABLE_PIN, 1);

	sequence.steps[0][SEQ_TEMP] = 100;
	sequence.steps[0][SEQ_DUR] = 30;

	sequence.steps[1][SEQ_TEMP] = 140;
	sequence.steps[1][SEQ_DUR] = 60;

	sequence.steps[2][SEQ_TEMP] = 170;
	sequence.steps[2][SEQ_DUR] = 75;

	sequence.steps[3][SEQ_TEMP] = 180;
	sequence.steps[3][SEQ_DUR] = 75;

	sequence.steps[4][SEQ_TEMP] = 215;
	sequence.steps[4][SEQ_DUR] = 15;

	sequence.steps[5][SEQ_TEMP] = 220;
	sequence.steps[5][SEQ_DUR] = 15;

	sequence.steps[6][SEQ_TEMP] = 230;
	sequence.steps[6][SEQ_DUR] = 60;

	sequence.steps[7][SEQ_TEMP] = 230;
	sequence.steps[7][SEQ_DUR] = 15;

	sequence.steps[8][SEQ_TEMP] = 200;
	sequence.steps[8][SEQ_DUR] = 75;

	sequence.steps[9][SEQ_TEMP] = 120;
	sequence.steps[9][SEQ_DUR] = 30;

	sequence.galden_temp = 235;
	sequence.anti_condensation_temp = 120;

	while (1) {
		systime_worker();
		cli_worker();
		temp_sensors_worker();
		lid_worker();
		lift_worker();
		seq_worker();
		ui_lcd_worker();

		//check if all temperature sensors were updated
		if (temp_sensor_cycle_complete) {
			temp_sensor_cycle_complete = 0;

			heater_worker();
		}
	}


	return 0;
}

static void init_system() {
	///////////////////////////
	//init system

	//enable Prefetch Buffer and set Flash Latency
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;

	//configure interrupt system
	NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUPING);

	//initiate core clocks
	core_clock_init();
}

static void init_peripherals() {
	///////////////////////////
	//init peripherals

	gpio_init();
	systime_init();
	comm_usart_init();
	cli_init();

	spi2.perif = SPI2;
	spi2.dma_tx.ch = DMA1_Channel5;
	spi2.dma_tx.iqrn = DMA1_Channel5_IRQn;
	spi2.dma_rx.ch = DMA1_Channel4;
	spi2.dma_rx.iqrn = DMA1_Channel4_IRQn;
	spi_init(&spi2);

	spi3.perif = SPI3;
	spi3.dma_tx.ch = DMA1_Channel3;
	spi3.dma_tx.iqrn = DMA1_Channel3_IRQn;
	spi3.dma_rx.ch = DMA1_Channel2;
	spi3.dma_rx.iqrn = DMA1_Channel2_IRQn;
	spi_init(&spi3);

	exti_init();
	pwm_init_TIM1();
	pwm_init_TIM2();
}

static void init_modules() {
	///////////////////////////
	//init modules

	cli_set_execute_cb(&cli_commands_execute);
	cli_set_broadcaster_cb(&cli_commands_broadcast);

	temp_sensors_init();

	//disable display chip select
	spi_stop(&lcd_top.spid);

	fans_init();

	axis_init(&lid_axis);
	axis_cal(&lid_axis, BWD);

	axis_init(&lift_axis);
	axis_cal(&lift_axis, FWD);

	heater_init(&TSNS_0);
	heater_regulation_disable();

	regulator_init(&TSNS_2, &lift_axis);
}

void main_millis_sync() {
	//check if broadcasting delay has passed
	if (broadcaster_delay) {
		--broadcaster_delay;
	} else {
		broadcaster_delay = BROADCASTER_DELAY_MS;

		//trigger next broadcasting cycle
		cli_broadcast_trigger();
	}

	static int ms;
	if (++ms > 100) {
		regulator_compute();
		ms = 0;
	}
}

void main_seconds_sync() {

}
