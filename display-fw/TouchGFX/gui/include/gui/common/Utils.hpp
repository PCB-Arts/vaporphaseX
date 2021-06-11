#ifndef GUI_UTILS_HPP
#define GUI_UTILS_HPP

#include <touchgfx/hal/HAL.hpp>

#ifdef SIMULATOR
#include <stdlib.h>
#endif

class Utils
{
public:
    static int randomNumberBetween(int lowest, int highest)
    {
#ifdef SIMULATOR
        return (int)(lowest + (highest - lowest) * (rand() / (float)RAND_MAX));
#else
        uint32_t random = (touchgfx::HAL::getInstance()->getCPUCycles() * HAL::getInstance()->getCPUCycles());
        return lowest + (random % (highest - lowest));
#endif
    }
};

#endif /* GUI_UTILS_HPP */
