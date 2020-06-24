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
#include "system/core_clock.h"

#include <stm32f3xx.h>

//=================================
// definitions

void core_clock_init() {
	// HCLK = SYSCLK
	// PCLK1 = HCLK / 2
	// PCLK2 = HCLK
	MODIFY_REG(RCC->CFGR,
		////////// clear bits //////////
		//clear AHB prescaler bits
		RCC_CFGR_HPRE |

		//clear APB1 prescaler bits
		RCC_CFGR_PPRE1 |

		//clear APB2 prescaler bits
		RCC_CFGR_PPRE2,

		////////// set bits //////////
		//set AHB prescaler to 1
		RCC_CFGR_HPRE_DIV1 |

		//set PCLK1 prescaler to 2
		RCC_CFGR_PPRE1_DIV2 |

		//set PCLK2 prescaler to 1
		RCC_CFGR_PPRE1_DIV1);


	//enable IO register clock´s
	SET_BIT(RCC->AHBENR,
		//GPIO A
		RCC_AHBENR_GPIOAEN |

		//GPIO B
		RCC_AHBENR_GPIOBEN |

		//GPIO C
		RCC_AHBENR_GPIOCEN |

		//GPIO D
		RCC_AHBENR_GPIODEN |

		//GPIO F
		RCC_AHBENR_GPIOFEN |

		//DMA
		RCC_AHBENR_DMA1EN);

	//enable system peripheral clock
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);

	//disable the HSE oscillator
	CLEAR_BIT(RCC->CR, RCC_CR_HSEON);

	SET_BIT(RCC->CR,
		//enable clock security system
		RCC_CR_CSSON |

		//bypass the HSE oscillator with external clock source
		RCC_CR_HSEBYP |

		//enable HSE oscillator
		RCC_CR_HSEON);

	//wait until HSE Osc is ready
	while((RCC->CR & RCC_CR_HSERDY) == 0);

	//PLL configuration
	MODIFY_REG(RCC->CFGR,
		////////// clear bits //////////
		//HSI/2 used as PREDIV1 entry and PREDIV1 forced to div by 2.
		RCC_CFGR_PLLSRC |

		//disable HSE PLL prescaler
		RCC_CFGR_PLLXTPRE |

		//reset PLL multiplier bits
		RCC_CFGR_PLLMUL,

		////////// set bits //////////
		//HSE used as PREDIV1 entry
		RCC_CFGR_PLLSRC_HSE_PREDIV |

		//HSE input to PLL not divided
		RCC_CFGR_PLLXTPRE_HSE_PREDIV_DIV1 |

		//set PLL multiplier to 6x
		RCC_CFGR_PLLMUL6);

	//enable PLL
	SET_BIT(RCC->CR, RCC_CR_PLLON);

	//wait until PLL is ready
	while((RCC->CR & RCC_CR_PLLRDY) == 0);

	//select PLL as system clock source
	MODIFY_REG(RCC->CFGR,
		////////// clear bits //////////
		//System clock switch
		RCC_CFGR_SW,

		////////// set bits //////////
		//PLL selected as system clock
		RCC_CFGR_SW_PLL);


	//wait till PLL is used as system clock source
	while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	//disable HSI oscillator
	CLEAR_BIT(RCC->CR, RCC_CR_HSION);

	//update system frequency
	SystemCoreClockUpdate();
}
