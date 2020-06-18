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


#ifndef MODULES_MAX31856_DEF_H_
#define MODULES_MAX31856_DEF_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies


//=================================
// declarations

#define MAX31856_REG_READ	0x00
#define MAX31856_REG_WRITE	0x80

//Register Memory Map
#define MAX31856_ADR_CR0	0x00
#define MAX31856_ADR_CR1	0x01
#define MAX31856_ADR_MASK	0x02
#define MAX31856_ADR_CJHF	0x03
#define MAX31856_ADR_CJLF	0x04
#define MAX31856_ADR_LTHFT	0x05
#define MAX31856_ADR_LTLFT	0x07
#define MAX31856_ADR_CJTO	0x09
#define MAX31856_ADR_CJT	0x0A
#define MAX31856_ADR_LTC	0x0C
#define MAX31856_ADR_SR		0x0F

//register defaults
#define MAX31856_CR0_DEFAULT	0x00
#define MAX31856_CR1_DEFAULT	0x03
#define MAX31856_MASK_DEFAULT	0xFF
#define MAX31856_CJHF_DEFAULT	0x7F
#define MAX31856_CJLF_DEFAULT	0xC0
#define MAX31856_LTHFTH_DEFAULT	0x7F
#define MAX31856_LTHFTL_DEFAULT 0xFF
#define MAX31856_LTLFTH_DEFAULT	0x80
#define MAX31856_LTLFTL_DEFAULT	0x00
#define MAX31856_CJTO_DEFAULT	0x00
#define MAX31856_CJTH_DEFAULT	0x00
#define MAX31856_CJTL_DEFAULT	0x00

//Configuration 0 Register (CR0)
#define MAX31856_CR0_CMODE	0b10000000
#define MAX31856_CR0_1SHOT	0b01000000
#define MAX31856_CR0_OCFAULT	0b00110000
#define MAX31856_CR0_CJ		0b00001000
#define MAX31856_CR0_FAULT	0b00000100
#define MAX31856_CR0_FAULTCLR	0b00000010
#define MAX31856_CR0_5060HZ	0b00000001

//Configuration 1 Register (CR1)
#define MAX31856_CR1_AVGSEL	0b01110000
#define MAX31856_CR1_TCTYPE	0b00001111

//Fault Mask Register (MASK)
#define MAX31856_MASK_CJHIGH	0b00100000
#define MAX31856_MASK_CJLOW	0b00010000
#define MAX31856_MASK_TCHIGH	0b00001000
#define MAX31856_MASK_TCLOW	0b00000100
#define MAX31856_MASK_OVUV_FLT	0b00000010
#define MAX31856_MASK_OPEN_FLT	0b00000001

//Fault Status Register (SR)
#define MAX31856_SR_CJ_RANGE	0b10000000
#define MAX31856_SR_TC_RANGE	0b01000000
#define MAX31856_SR_CJHIGH	0b00100000
#define MAX31856_SR_CJLOW	0b00010000
#define MAX31856_SR_TCHIGH	0b00001000
#define MAX31856_SR_TCLOW	0b00000100
#define MAX31856_SR_OVUV_FLT	0b00000010
#define MAX31856_SR_OPEN_FLT	0b00000001

#define MAX31856_RESOLUTION	0.0078125f


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_MAX31856_DEF_H_ */
