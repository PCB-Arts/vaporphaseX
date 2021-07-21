[#ftl]
/**
  ******************************************************************************
  * File Name          : ${name.split("/")?last}
  ******************************************************************************
[@common.optinclude name=mxTmpFolder+"/license.tmp"/][#--include License text --]
  ******************************************************************************
  */
#n
#include <touchgfx/hal/GPIO.hpp>
#n
/**
 * GPIO_ID Enum, these are used bt TouchGFX framework to signal events.
 *
 * VSYNC_FREQ,  /// Pin is toggled at each VSYNC
 * RENDER_TIME, /// Pin is high when frame rendering begins, low when finished
 * FRAME_RATE,  /// Pin is toggled when the frame buffers are swapped.
 * MCU_ACTIVE   /// Pin is high when framework is utilizing the MCU.
 *
 */

/* USER CODE BEGIN ${name?split("/")?last} */
#n
using namespace touchgfx;

/*
 * Perform configuration of IO pins.
 */
void GPIO::init()
{

}

/*
 * Sets a pin high.
 */
void GPIO::set(GPIO_ID id)
{

}

/*
 * Sets a pin low.
 */
void GPIO::clear(GPIO_ID id)
{

}

/*
 * Toggles a pin.
 */
void GPIO::toggle(GPIO_ID id)
{

}

/*
 * Gets the state of a pin.
 */
bool GPIO::get(GPIO_ID id)
{
  return false;
}

/* USER CODE END ${name?split("/")?last} */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/