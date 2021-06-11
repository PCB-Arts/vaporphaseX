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

#include "packet_handler.h"
#include "vpo_protocol.h"
#include "third_party.h"

static bool ThirdParty_allowed = false;

int ThirdParty_Init() {
	union prop_value prop;
	int rc = usart_sync_prop_get(VPO_PROP_THIRD_PARTY_SOFTWARE, &prop);
	if (rc != PRES_OK) {
		return rc;
	}

	ThirdParty_allowed = prop.bl;

	return 0;
}

bool ThirdParty_Allowed() {
	return ThirdParty_allowed;
}

