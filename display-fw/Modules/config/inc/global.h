#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "stm32u5xx_hal.h"

#define ARRAY_LEN(x)                                                           \
  ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#endif /*__GLOBAL_H__*/
