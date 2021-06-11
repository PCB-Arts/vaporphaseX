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

#include "limit_switch.h"

#include "global.h"

void limit_switch_init(limit_switch_t* limit_switch) {
    // if the pin is set on init the limit switch should be active
    limit_switch->triggered = iopin_read(limit_switch->pin) == GPIO_PIN_SET;
    limit_switch->triggered_timestamp_ms = HAL_GetTick() - limit_switch->trigger_propagation_delay_ms;
}

// returns true if limit switch was triggered and remained active for delay period
bool limit_switch_active(limit_switch_t* limit_switch) {
    if(!limit_switch->triggered) {
        return false;
    }

    if(iopin_read(limit_switch->pin) != GPIO_PIN_SET) {
        // reset triggered state
        limit_switch->triggered = false;
        return false;
    }

    uint32_t time_since_trigger = HAL_GetTick() - limit_switch->triggered_timestamp_ms;

    return time_since_trigger >= limit_switch->trigger_propagation_delay_ms;
}

void limit_switch_triggered_isr(limit_switch_t* limit_switch) {
    limit_switch->triggered = true;
    limit_switch->triggered_timestamp_ms = HAL_GetTick();
}
