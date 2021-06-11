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

#ifndef __CORE_COMMUNICATION_H
#define __CORE_COMMUNICATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "vpo_protocol.h"
#include "packet_handler.h"

void CoreCom_Init();

// properties

int32_t CoreCom_CoreOperationTime();

uint64_t CoreCom_PcbTemperature();
uint64_t CoreCom_LiquidTemperature();
uint64_t CoreCom_CoolantTemperature();

Vpo_CoreStateTypeDef CoreCom_CoreState();

void CoreCom_StartReflow();
void CoreCom_StopReflow();

void CoreCom_OpenLid();
void CoreCom_CloseLid();

void CoreCom_SetLidMode(LidOpenMode mode);

typedef struct __CoreCom_ErrorState {
	bool communication_error: 1;
} CoreCom_ErrorState;

CoreCom_ErrorState CoreCom_GetErrorState();

void CoreCom_ReadMonitor();
bool CoreCom_HasNewMonitor();
Vpo_CoreMonitorStateComplete CoreCom_GetMonitorState();

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#include <ProfileParser.hpp>

int usart_send_profile_frame(const ProfileTransmissionPacket *in_buf);
bool CoreCom_StartProfile(SolderProfile& profile);
#endif

void CoreCom_StartSelftest();

#endif
