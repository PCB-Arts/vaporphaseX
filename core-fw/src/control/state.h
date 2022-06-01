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

#ifndef __STATE_MODULE_H
#define __STATE_MODULE_H

#include <stdint.h>
#include <stdbool.h>

#include "vpo_types.h"

void State_Init();
void State_SetReflow(bool active);
void State_SetLidOpen(bool open);
void State_Worker();
void State_SetLidOpenMode(LidOpenMode mode);
void State_StartSelftest();

Vpo_CoreStateTypeDef State_getState();
Vpo_CoreMonitorState1 State_getMonitor1State();
Vpo_CoreMonitorState2 State_getMonitor2State();
Vpo_CoreMonitorState3 State_getMonitor3State();
CurrentState State_CurrentState();

#endif
