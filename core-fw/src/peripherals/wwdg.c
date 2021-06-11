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

#include "wwdg.h"

#include <stm32f3xx.h>

void wwdg_init(void) {
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_WWDG_STOP;
	WWDG->CFR = WWDG_CFR_EWI | (3 << WWDG_CFR_WDGTB_Pos) | (0xff << WWDG_CFR_W_Pos);
	WWDG->CR  = WWDG_CR_WDGA | (0xfe << WWDG_CR_T_Pos);

	NVIC_SetPriority(WWDG_IRQn, 0);
	NVIC_EnableIRQ(WWDG_IRQn);
}

void wwdg_reset(void) {
	WWDG->CR  = WWDG_CR_WDGA | (0xfe << WWDG_CR_T_Pos);
}
