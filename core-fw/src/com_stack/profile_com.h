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

#ifndef COM_STACK_PROFILE_COM_H_
#define COM_STACK_PROFILE_COM_H_
#include <stdint.h>

#include "packet_handler.h"

typedef int16_t seconds;
typedef int16_t celsius;

typedef struct TemperatureTimeDatapoint {
	celsius temperature;
	seconds time;
} TemperatureTimeDatapoint;

typedef struct TemperatureProfile{
	TemperatureTimeDatapoint datapoints[600];
	int8_t profileId;
	int8_t tempProfileId;
	int16_t totalDatapoints;
	int16_t availableDatapoints;
} TemperatureProfile;

extern TemperatureProfile globalProfile;

void profile_com_init();
int app_profile_frame_received(ProfileTransmissionPacket *packet);

#endif /* COM_STACK_PROFILE_COM_H_ */
