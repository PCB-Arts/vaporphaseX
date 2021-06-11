#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "stm32f3xx_hal.h"

#define ARRAY_LEN(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#endif/*__GLOBAL_H__*/
