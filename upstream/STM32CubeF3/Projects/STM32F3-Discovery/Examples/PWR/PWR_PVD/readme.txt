/**
  @page PWR_PVD Power Programmable Voltage Detector (PVD) example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    PWR/PWR_PVD/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR Programmable Voltage Detector (PVD) example
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description 
How to configure the programmable voltage detector by using an external interrupt 
line. External DC supply must be used to supply Vdd.

In this example, EXTI line 16 is configured to generate 
an interrupt on each rising or falling edge of the PVD output signal (which 
indicates that the Vdd voltage is moving below or above the PVD threshold).
As long as the voltage is above the target threshold, LED3 is blinking; 
when the voltage drops below the threshold, LED3 stops blinking and remains 
constantly on (or appears to be turned off if the voltage is getting really low);
when the voltage moves back above the target threshold, LED3 starts blinking again.
 
If LED4 is on, it means that the system has generated an error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@par Directory contents 

  - PWR/PWR_PVD/Inc/stm32f3xx_conf.h         Library Configuration file
  - PWR/PWR_PVD/Inc/stm32f3xx_it.h           Header for stm32f3xx_it.c
  - PWR/PWR_PVD/Inc/main.h                   Header file for main.c
  - PWR/PWR_PVD/Src/system_stm32f3xx.c       STM32F3xx system clock configuration file
  - PWR/PWR_PVD/Src/stm32f3xx_it.c           Interrupt handlers
  - PWR/PWR_PVD/Src/main.c                   Main program
  - PWR/PWR_PVD/Src/stm32f3xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32F303xC devices
    
      
  - This example has been tested with STMicroelectronics STM32F3-Discovery RevB
    board and can be easily tailored to any other supported device 
    and development board.

  - STM32F3-Discovery RevB Set-up
    - apply an external voltage on the proper connector pins

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
