/**
  @page EEPROM_Emulation application to show an eeprom emulation

  @verbatim
  ******************* (C) COPYRIGHT 2016 STMicroelectronics ********************
  * @file    EEPROM_Emulation/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the EEPROM_Emulation application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Application Description 

This application shows how to emulate EEPROM on internal flash.

Please refer to AN2594 for futher details regarding this application.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 


 - EEPROM_Emulation/Inc/stm32f3xx_hal_conf.h        HAL Configuration file
 - EEPROM_Emulation/Inc/eeprom.h                      Header for main.c module 
 - EEPROM_Emulation/Inc/stm32f3xx_it.h              Header for stm32f3xx_it.c
 - EEPROM_Emulation/Src/main.c                      Main program
 - EEPROM_Emulation/Src/eeprom.c                      EEPROM program
 - EEPROM_Emulation/Src/stm32f3xx_it.c              Interrupt handlers
 - EEPROM_Emulation/Src/system_stm32f3xx.c          STM32F3xx system clock configuration file 
    

@par Hardware and Software environment 

  - This example runs on STM32F3xx devices.
    
  - This example has been tested with STM32373C-EVAL RevB board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
