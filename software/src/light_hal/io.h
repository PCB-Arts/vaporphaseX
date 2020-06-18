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

#ifndef LIGHT_HAL_IO_H_
#define LIGHT_HAL_IO_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */

//=================================
// included dependencies
#include <stm32f3xx.h>

#include <stdint.h>

//=================================
// declarations

#define IO_CONFIG_MODE_MASK		0b11U
#define IO_CONFIG_AFR_MASK		0b111U
#define IO_CONFIG_SPEED_MASK	0b11U
#define IO_CONFIG_TYPE_MASK		0b1U
#define IO_CONFIG_PULL_MASK		0b11U

#define IO_MODE_INPUT			0b00U
#define IO_MODE_OUTPUT			0b01U
#define IO_MODE_FUNCTION		0b10U
#define IO_MODE_ANALOG			0b11U

#define IO_TYPE_PUSH_PULL		0b0U
#define IO_TYPE_OPEN_DRAIN		0b1U

#define IO_SPEED_LOW			0b00U
#define IO_SPEED_MEDIUM			0b01U
#define IO_SPEED_HIGH			0b11U

#define IO_PULL_NONE			0b00U
#define IO_PULL_UP				0b01U
#define IO_PULL_DOWN			0b10U

#define IO_AF00				0b0000U
#define IO_AF01				0b0001U
#define IO_AF02				0b0010U
#define IO_AF03				0b0011U
#define IO_AF04				0b0100U
#define IO_AF05				0b0101U
#define IO_AF06				0b0110U
#define IO_AF07				0b0111U
#define IO_AF08				0b1000U
#define IO_AF09				0b1001U
#define IO_AF10				0b1010U
#define IO_AF11				0b1011U
#define IO_AF12				0b1100U
#define IO_AF13				0b1101U
#define IO_AF14				0b1110U
#define IO_AF15				0b1111U

#define IO_PIN00 			0b0000U
#define IO_PIN01 			0b0001U
#define IO_PIN02 			0b0010U
#define IO_PIN03 			0b0011U
#define IO_PIN04 			0b0100U
#define IO_PIN05 			0b0101U
#define IO_PIN06 			0b0110U
#define IO_PIN07 			0b0111U
#define IO_PIN08 			0b1000U
#define IO_PIN09 			0b1001U
#define IO_PIN10			0b1010U
#define IO_PIN11			0b1011U
#define IO_PIN12			0b1100U
#define IO_PIN13			0b1101U
#define IO_PIN14			0b1110U
#define IO_PIN15			0b1111U

#define IO_MASK(__IO_PIN__) (0b1U << __IO_PIN__)

#define SET_IO_MODE(PORT, PIN, MODE) PORT->MODER =\
	(PORT->MODER & ~(IO_CONFIG_MODE_MASK << (PIN * 2U))) |\
	(MODE << (PIN * 2U))

#define SET_IO_AFR(PORT, PIN, FUNCTION) PORT->AFR[PIN >> 3] =\
	(PORT->AFR[PIN >> 3] & ~(0xFU << ((uint32_t)(PIN & IO_CONFIG_AFR_MASK) * 4U))) |\
	((uint32_t)(FUNCTION) << (((uint32_t)PIN & IO_CONFIG_AFR_MASK) * 4U))

#define SET_IO_SPEED(PORT, PIN, SPEED) PORT->OSPEEDR =\
	(PORT->OSPEEDR & ~(IO_CONFIG_SPEED_MASK << (PIN * 2U))) |\
	(SPEED << (PIN * 2U))

#define SET_IO_TYPE(PORT, PIN, MODE) PORT->OTYPER =\
	(PORT->OTYPER & ~(IO_CONFIG_TYPE_MASK << PIN)) |\
	(MODE << PIN)

#define SET_IO_PULL(PORT, PIN, PULL) PORT->PUPDR =\
	(PORT->PUPDR & ~(IO_CONFIG_PULL_MASK << (PIN * 2U))) |\
	(PULL << (PIN * 2U))

typedef struct
{
	uint32_t Pin;
	uint32_t Mode;
	uint32_t Pull;
	uint32_t Speed;
	uint32_t Type;
	uint32_t Alternate;
} IO_TypeDef;

typedef struct {
	const uint32_t mask;
	GPIO_TypeDef* const port;
} IO_pin_t;

#define IO_PIN_TPL(port, pin) {pin, port}

#define IO_READ_PIN(PORT, PIN) (((PORT)->IDR & (0b1 << (PIN))) != 0)

extern void io_config(GPIO_TypeDef *GPIOx, const IO_TypeDef* pin_config);
extern void io_write_pin(GPIO_TypeDef *GPIOx, const uint32_t Pin, const uint8_t pin_state);
extern void io_write_pins(GPIO_TypeDef *GPIOx, const uint32_t pin_mask, const uint8_t pin_state);
extern void io_toggle_pins(GPIO_TypeDef *GPIOx, const uint32_t pin_mask);

#define IOPIN_WRITE(IO_pin, state) io_write_pin(IO_pin.port, IO_pin.mask, state)
#define IOPIN_READ(IO_pin) IO_READ_PIN(IO_pin.port, IO_pin.mask)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIGHT_HAL_IO_H_ */
