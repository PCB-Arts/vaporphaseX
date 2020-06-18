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
#include "light_hal/io.h"

//=================================
// definitions

void io_config(GPIO_TypeDef *GPIOx, const IO_TypeDef* pin_config) {
	//mode register
	MODIFY_REG(
			GPIOx->MODER,
			IO_CONFIG_MODE_MASK << (pin_config->Pin * 2U),
			pin_config->Mode << (pin_config->Pin * 2U));

	//alternate function register
	MODIFY_REG(
			GPIOx->AFR[pin_config->Pin >> 3],
			0xFU << ((uint32_t)(pin_config->Pin & IO_CONFIG_AFR_MASK) * 4U),
			(uint32_t)(pin_config->Alternate) << (((uint32_t)pin_config->Pin & IO_CONFIG_AFR_MASK) * 4U));

	//speed register
	MODIFY_REG(
			GPIOx->OSPEEDR,
			IO_CONFIG_SPEED_MASK << (pin_config->Pin * 2U),
			pin_config->Speed << (pin_config->Pin * 2U));

	//type register
	MODIFY_REG(
			GPIOx->OTYPER,
			IO_CONFIG_TYPE_MASK << pin_config->Pin,
			pin_config->Type << pin_config->Pin);

	//pull up/down register
	MODIFY_REG(
			GPIOx->PUPDR,
			IO_CONFIG_PULL_MASK << (pin_config->Pin * 2U),
			pin_config->Pull << (pin_config->Pin * 2U));
}

void io_write_pin(GPIO_TypeDef *GPIOx, const uint32_t Pin, const uint8_t pin_state){
	if(pin_state)
		GPIOx->BSRR = 0b1U << Pin;
	else
		GPIOx->BRR = 0b1U << Pin;
}

void io_write_pins(GPIO_TypeDef *GPIOx, const uint32_t pin_mask, const uint8_t pin_state){
	if(pin_state)
		GPIOx->BSRR = pin_mask;
	else
		GPIOx->BRR = pin_mask;
}

void io_toggle_pins(GPIO_TypeDef *GPIOx, const uint32_t pin_mask){
	GPIOx->ODR ^= pin_mask;
}
