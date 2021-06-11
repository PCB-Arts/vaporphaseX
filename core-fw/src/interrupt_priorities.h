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
#define NVIC_PRIO_SAFETY		NVIC_PRIO_TPL(1,0)
#define NVIC_PRIO_LIMSW			NVIC_PRIO_TPL(5,0)
#define NVIC_PRIO_COMMAND_USART		NVIC_PRIO_TPL(4,0)
#define NVIC_PRIO_OTP_STATUS		NVIC_PRIO_TPL(5,0)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INTERRUPT_PRIORITIES_H_ */
