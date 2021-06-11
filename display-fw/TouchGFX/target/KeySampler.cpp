/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.8.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * Copyright (C) 2014-2016 Draupner Graphics A/S <http://www.touchgfx.com>.
 * All rights reserved.
 *
 * TouchGFX is protected by international copyright laws and the knowledge of
 * this source code may not be used to write a similar product. This file may
 * only be used in accordance with a license and should not be re-
 * distributed in any way without the prior permission of Draupner Graphics.
 *
 * This is licensed software for evaluation use, any use must strictly comply
 * with the evaluation license agreement provided with delivery of the
 * TouchGFX software.
 *
 * The evaluation license agreement can be seen on www.touchgfx.com
 *
 * @section Disclaimer
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
 * no obligation to support this software. Draupner Graphics A/S is providing
 * the software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Draupner Graphics A/S can not be held liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this software.
 *
 *****************************************************************************/
#include "KeySampler.hpp"
#include "touchgfx/hal/Buttons.hpp"
#include "string.h"

using namespace touchgfx;

static uint8_t btnstatus[4];

void KeySampler::init()
{
    Buttons::init();

    memset(btnstatus, 0x00, sizeof(uint8_t) * 4);
}

bool KeySampler::sample(uint8_t& key)
{
    unsigned int buttonValue = Buttons::sample();

    //Propergate values through buffer
    btnstatus[3] = btnstatus[2];
    btnstatus[2] = btnstatus[1];
    btnstatus[1] = btnstatus[0];
    btnstatus[0] = buttonValue;

    //User key
    if (((btnstatus[3] & Key1) == 1) && ((btnstatus[2] & Key1) == 1) &&
            ((btnstatus[1] & Key1) == 1) && ((btnstatus[0] & Key1) == 0))
    {
        key = 1;
        return true;
    }

    return false;
}
