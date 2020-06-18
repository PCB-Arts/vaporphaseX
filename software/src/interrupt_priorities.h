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


#ifndef INTERRUPT_PRIORITIES_H_
#define INTERRUPT_PRIORITIES_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies

//=================================
// declarations

//3 bits for preemp priority, 1 bit for sub priority
#define NVIC_PRIORITY_GROUPING		1U

//shift the preempt priority left by number of sub priority bits and or sub bits
#define NVIC_PRIO_TPL(preempt, sub) ((preempt << NVIC_PRIORITY_GROUPING) | sub)

//lowest group number => highest priority

#define NVIC_PRIO_SYSTIME		NVIC_PRIO_TPL(0,0)
#define NVIC_PRIO_ENCODERS		NVIC_PRIO_TPL(2,0)
#define NVIC_PRIO_LIMSW			NVIC_PRIO_TPL(5,0)
#define NVIC_PRIO_COMMAND_USART		NVIC_PRIO_TPL(4,0)
#define NVIC_PRIO_OTP_STATUS		NVIC_PRIO_TPL(5,0)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INTERRUPT_PRIORITIES_H_ */
