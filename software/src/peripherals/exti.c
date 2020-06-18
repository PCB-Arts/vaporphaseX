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
#include "peripherals/exti.h"
#include "interrupt_priorities.h"

#include <stm32f3xx.h>

//=================================
// definitions

void exti_init() {
	SET_BIT(SYSCFG->EXTICR[0],
		//enable port C EXTI0 line
		// - lift top limit switch
		SYSCFG_EXTICR1_EXTI0_PC |

		//enable port C EXTI1 line
		// - lift bottom limit switch
		SYSCFG_EXTICR1_EXTI1_PC |

		//enable port C EXTI2 line
		// - lid top limit switch
		SYSCFG_EXTICR1_EXTI2_PC |

		//enable port C EXTI3 line
		// - lid bottom limit switch
		SYSCFG_EXTICR1_EXTI3_PC);

	SET_BIT(SYSCFG->EXTICR[1],
		//enable port A EXTI4 line
		// - heater OTP status
		SYSCFG_EXTICR2_EXTI4_PA |

		//enable port B EXTI6 line
		// - lid encoder 1
		SYSCFG_EXTICR2_EXTI6_PB |

		//enable port B EXTI7 line
		// - lid encoder 2
		SYSCFG_EXTICR2_EXTI7_PB);

	SET_BIT(SYSCFG->EXTICR[2],
		//enable port B EXTI8 line
		// - lift encoder 1
		SYSCFG_EXTICR3_EXTI8_PB |

		//enable port B EXTI9 line
		// - lift encoder 2
		SYSCFG_EXTICR3_EXTI9_PB);

	//enable rising edge detector
	SET_BIT(EXTI->RTSR,
		//EXTI0
		// - lift top limit switch
		EXTI_RTSR_TR0 |

		//EXTI1
		// - lift bottom limit switch
		EXTI_RTSR_TR1 |

		//EXTI2
		// - lid top limit switch
		EXTI_RTSR_TR2 |

		//EXTI3
		// - lid bottom limit switch
		EXTI_RTSR_TR3 |

		//EXTI4
		// - OTP status
		EXTI_RTSR_TR4 |

		//EXTI16
		// - lid encoder 1
		EXTI_RTSR_TR6 |

		//EXTI7
		// - lid encoder 2
		EXTI_RTSR_TR7 |

		//EXTI8
		// - lift encoder 1
		EXTI_RTSR_TR8 |

		//EXTI9
		// - lift encoder 2
		EXTI_RTSR_TR9);

	//enable falling edge detector
	SET_BIT(EXTI->FTSR,
		//EXTI16
		// - lid encoder 1
		EXTI_FTSR_TR6 |

		//EXTI7
		// - lid encoder 2
		EXTI_FTSR_TR7 |

		//EXTI8
		// - lift encoder 1
		EXTI_FTSR_TR8 |

		//EXTI9
		// - lift encoder 2
		EXTI_FTSR_TR9);

	//set NVIC priority
	NVIC_SetPriority(EXTI9_5_IRQn, NVIC_PRIO_ENCODERS);
	NVIC_SetPriority(EXTI0_IRQn, NVIC_PRIO_LIMSW);
	NVIC_SetPriority(EXTI1_IRQn, NVIC_PRIO_LIMSW);
	NVIC_SetPriority(EXTI2_TSC_IRQn, NVIC_PRIO_LIMSW);
	NVIC_SetPriority(EXTI3_IRQn, NVIC_PRIO_LIMSW);
	NVIC_SetPriority(EXTI4_IRQn, NVIC_PRIO_OTP_STATUS);

	//enable encoder interrupt line
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_TSC_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);

	//enable EXTI interrupts
	SET_BIT(EXTI->IMR,
		//lift limit switch top
		EXTI_IMR_MR0 |

		//lift limit switch bottom
		EXTI_IMR_MR1 |

		//lid limit switch top
		EXTI_IMR_MR2 |

		//lid limit switch bottom
		EXTI_IMR_MR3 |

		//OTP status
		EXTI_IMR_MR4 |

		//lid encoder 1
		EXTI_IMR_MR6 |

		//lid encoder 2
		EXTI_IMR_MR7 |

		//lift encoder 1
		EXTI_IMR_MR8 |

		//lift encoder 2
		EXTI_IMR_MR9);
}
