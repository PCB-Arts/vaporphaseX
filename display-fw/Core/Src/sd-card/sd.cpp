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

#include <cmsis_os.h>
#include <sd-card/sd.hpp>
#include <stdbool.h>

#include "sd-card/memory_card.h"
#include "sd-card/sd_diskio.h"

#include "log.h"

#include "ProfileParser.hpp"

// forward declaration of private functions in logging.c
#ifdef __cplusplus
extern "C" {
#endif
void SD_Log_Init();
void SD_Log_Worker();
#ifdef __cplusplus
}
#endif

osThreadId_t SD_TaskHandle;
const osThreadAttr_t SD_Task_attributes = []{
	osThreadAttr_t attr;
	attr.name = "SDTask";
	attr.stack_size =  2048;
    attr.priority = (osPriority_t)osPriorityLow;
    return attr;
}();


int DataLoadCounter= -1;
std::vector<std::string> AvailableCSVProfiles;


static void SD_Task(void *argument);

volatile SD_ErrorTypeDef SD_error_state;
static volatile bool SD_mounted = false;
static volatile bool SD_should_unmount = false;

void SD_Init() {
	BSP_SD_Detect_MspInit(NULL, NULL);

	SD_TaskHandle =
	      osThreadNew(SD_Task, NULL, &SD_Task_attributes);

	SD_Log_Init();
}

void SD_Mount() {
	bool mount_success = sd_mount() == 0;
	vpo_log("SD mount status: %s", mount_success ? "true" : "false");

	SD_error_state.mount_failed = !mount_success;
	SD_error_state.removed_without_unmount = false;
	SD_mounted = mount_success;

	if (!mount_success){
		sd_unmount();
		return;
	}

	AvailableCSVProfiles.clear();
	auto files = FindCSVFiles();
	vpo_log("Found valid CSV files");
	for(auto file: files){
		if(IsSolderprofileCSV(file)){
			vpo_log("	%s", file.c_str());

			AvailableCSVProfiles.push_back(file);
		}
	}
	DataLoadCounter++;
}

static void SD_Unmount() {
	vpo_log("SD ejected");
	SD_mounted = false;
	SD_should_unmount = false;
	sd_log_write_line(100, "x - DISP: SD unmounted.");

	DataLoadCounter++;
	AvailableCSVProfiles.clear();

	sd_unmount();
}


static void SD_Mount_Worker() {
	if (SD_mounted) {
		if (SD_should_unmount) {
			SD_Unmount();
		}
	}
}

static void SD_Task(void *argument) {
	if (BSP_SD_IsDetected()) {
		SD_Mount();

		if (SD_mounted) {
			vpo_log("SD mounted on boot.");
		}
	}

	while (true) {
		SD_Mount_Worker();
		if (SD_mounted) {
			SD_Log_Worker();
		}

		osDelay(100);
	}
}

bool SD_Mounted() {
	return SD_mounted;
}

void SD_SheduleUnmount() {
	SD_should_unmount = true;
}


int GetFileLoadCounter(){
	return DataLoadCounter;
}

std::vector<std::string> GetAvailableCSVFiles()
{
	std::vector<std::string> copy(AvailableCSVProfiles);
	return copy;
}
