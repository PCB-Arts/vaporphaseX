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

#ifndef __EVENT_H__
#define __EVENT_H__

enum event_type {
    EVT_NONE,
    EVT_LID_MOTOR_OVERCURRENT,
	EVT_WATCHDOG, /* Hardware watchdog timer expired */
	EVT_FAULT, /* HardFault, BusFault, UsageFault */
	EVT_EMERGENCY_STOP, /* User-initiated emergency stop */
	EVT_WATER_OVERHEAT, /* Cooling water temperature too high during device startup */
    EVT_NUM_EVENTS
};

void event_notify(enum event_type type);
void event_notify_now(enum event_type type);

#endif /* __EVENT_H__ */
