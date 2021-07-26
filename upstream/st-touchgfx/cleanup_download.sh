#!/bin/sh
set -e

cd upstream

rm -r Utilities
rm -r Middlewares/ST/TouchGFX/touchgfx/lib/win
rm -r Middlewares/ST/TouchGFX/touchgfx/lib/core/cortex_m0
rm -r Middlewares/ST/TouchGFX/touchgfx/lib/core/cortex_m33
rm -r Middlewares/ST/TouchGFX/touchgfx/lib/core/cortex_m7
rm -r Drivers/BSP/Components/mx_wifi
