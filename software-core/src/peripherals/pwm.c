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
#include "peripherals/pwm.h"

#include <stm32f3xx.h>

//=================================
// defines

#define PWM_FREQ_TIM1 19000
#define PWM_FREQ_TIM2 25000

void pwm_init_TIM1() {
	//enable peripheral clock
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);

	//output disable
	CLEAR_BIT(TIM1->CCER,
			//CH 2
			TIM_CCER_CC2E |

			//CH 3
			TIM_CCER_CC3E);

	//set frequency
	TIM1->ARR = SystemCoreClock / PWM_FREQ_TIM1;

	//100% duty cycle
	TIM1->CCR2 = 100;
	TIM1->CCR3 = 100;

	//0110 = PWM mode 1
	MODIFY_REG(TIM1->CCMR1,
		////////// clear bits //////////
		//CH 2
		TIM_CCMR1_OC2M_0 |
		TIM_CCMR1_OC2M_3,

		////////// set bits //////////
		//CH 2
		TIM_CCMR1_OC2M_1 |
		TIM_CCMR1_OC2M_2 |

		//Output compare 2 preload enable
		TIM_CCMR1_OC2PE);

	//0110 = PWM mode 1
	MODIFY_REG(TIM1->CCMR2,
		////////// clear bits //////////
		//CH 3
		TIM_CCMR2_OC3M_0 |
		TIM_CCMR2_OC3M_3,

		////////// set bits //////////
		//CH 3
		TIM_CCMR2_OC3M_1 |
		TIM_CCMR2_OC3M_2 |

		//Output compare 3preload enable
		TIM_CCMR2_OC3PE);

	//set main output enable bit
	SET_BIT(TIM1->BDTR, TIM_BDTR_MOE);

	//auto reload register
	SET_BIT(TIM1->CR1,
		//Auto-reload preload enable
		TIM_CR1_ARPE |

		//Counter enabled
		TIM_CR1_CEN);

	//output enable
	SET_BIT(TIM1->CCER,
		//CH 3
		TIM_CCER_CC2E |

		//CH 4
		TIM_CCER_CC3E);
}

void pwm_init_TIM2() {
	//enable peripheral clock
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);

	//output disable
	CLEAR_BIT(TIM2->CCER,
			//CH 3
			TIM_CCER_CC3NE |

			//CH 4
			TIM_CCER_CC4E);

	//set frequency
	TIM2->ARR = SystemCoreClock / PWM_FREQ_TIM2;

	//100% duty cycle
	TIM2->CCR3 = 100;
	TIM2->CCR4 = 100;

	//0110 = PWM mode 1
	MODIFY_REG(TIM2->CCMR2,
		////////// clear bits //////////
		//CH 3
		TIM_CCMR2_OC3M_0 |
		TIM_CCMR2_OC3M_3 |

		//CH 4
		TIM_CCMR2_OC4M_0 |
		TIM_CCMR2_OC4M_3,

		////////// set bits //////////
		//CH 3
		TIM_CCMR2_OC3M_1 |
		TIM_CCMR2_OC3M_2 |

		//CH 4
		TIM_CCMR2_OC4M_1 |
		TIM_CCMR2_OC4M_2 |

		//Output compare 3 & 4 preload enable
		TIM_CCMR2_OC3PE |
		TIM_CCMR2_OC4PE);

	//set main output enable bit
	SET_BIT(TIM2->BDTR, TIM_BDTR_MOE);

	//auto reload register
	SET_BIT(TIM2->CR1,
		//Auto-reload preload enable
		TIM_CR1_ARPE |

		//Counter enabled
		TIM_CR1_CEN);

	//output enable
	SET_BIT(TIM2->CCER,
			//CH 3
			TIM_CCER_CC3E |

			//CH 4
			TIM_CCER_CC4E);
}

void pwm_set_TIM1CH2(int value) {
	float p = value / 100.0f;
	TIM1->CCR2 = TIM1->ARR * p;
}

void pwm_set_TIM1CH3(int value) {
	float p = value / 100.0f;
	TIM1->CCR3 = TIM1->ARR * p;
}

void pwm_set_TIM2CH3(int value) {
	float p = value / 100.0f;
	TIM2->CCR3 = TIM2->ARR * p;
}

void pwm_set_TIM2CH4(int value) {
	float p = value / 100.0f;
	TIM2->CCR4 = TIM2->ARR * p;
}
