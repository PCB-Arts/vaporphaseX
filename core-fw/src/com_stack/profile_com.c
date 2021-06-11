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

#include "profile_com.h"

TemperatureProfile globalProfile;

void profile_com_init() {
	globalProfile.profileId = -1;
	globalProfile.tempProfileId = -1;
}

int app_profile_frame_received(ProfileTransmissionPacket *packet)
{
	if(packet->profileId != globalProfile.tempProfileId && packet->entriesOffset == 0)
	{
		globalProfile.tempProfileId = packet->profileId;
		globalProfile.profileId = -1;
		globalProfile.totalDatapoints = packet->totalEntries;
		globalProfile.availableDatapoints = 0;
	}else if(packet->profileId != globalProfile.tempProfileId){
		return -1;
	}

	if(packet->entriesOffset!=globalProfile.availableDatapoints){
		return -1;
	}

	for(int i = 0; i < packet->numberEntries;i++){
		TemperatureTimeDatapoint point;
		point.temperature = packet->temperatures[i];
		point.time = packet->times[i];

		globalProfile.datapoints[globalProfile.availableDatapoints+i] = point;
	}
	globalProfile.availableDatapoints += packet->numberEntries;

	if(globalProfile.availableDatapoints == globalProfile.totalDatapoints){
		globalProfile.profileId = globalProfile.tempProfileId;
	}
	return 0;
}

