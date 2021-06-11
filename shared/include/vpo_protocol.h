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

#ifndef __VPO_PROTOCOL_H__
#define __VPO_PROTOCOL_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "vpo_types.h"

enum Vpo_Properties {
	VPO_PROP_REFLOW_TIME = 1, /* Time of the current reflow operation */
	VPO_PROP_TEMP_LIQUID = 2, /* Galden unit: 0.001 Degrees celsius, format: i32 */
	VPO_PROP_TEMP_COOLANT = 3, /* Coolant unit: 0.001 Degrees celsius, format: i32 */
	VPO_PROP_TEMP_PCB = 7,
	VPO_PROP_CORE_STATE = 4,
	VPO_PROP_REFLOW_ACTIVE = 5,
	VPO_PROP_LID_OPEN = 6,
	VPO_PROP_LID_MODE = 8,
	VPO_PROP_MONITOR_1 = 9,
	VPO_PROP_MONITOR_2 = 10,
	VPO_PROP_MONITOR_3 = 11,
	VPO_PROP_SELFTEST_ACTIVE = 12,

	VPO_PROP_THIRD_PARTY_SOFTWARE = 0xFF,
};


#ifdef __cplusplus
}
#endif
#endif /* __VPO_PROTOCOL_H__ */
