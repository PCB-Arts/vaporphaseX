[#ftl]
[#assign moduleName = "x-cube-gfx1"]
[#if ModuleName??]
	[#assign moduleName = ModuleName]
[/#if]
/**
  ******************************************************************************
  * File Name          : app_${moduleName?lower_case}.c
  ******************************************************************************
[@common.optinclude name=mxTmpFolder+"/license.tmp"/][#--include License text --]
  ******************************************************************************
  */

#include "app_${moduleName?lower_case}.h"

void touchgfx_init(void);
void touchgfx_taskEntry(void);

/**
 * Initialize TouchGFX application
 */
void ${fctName}(void)
{
  // Calling farward to touchgfx_init in C++ domain
  touchgfx_init();
}

/**
 * TouchGFX application entry function
 */
void ${fctProcessName}(void)
{
  // Calling farward to touchgfx_init in C++ domain
  touchgfx_taskEntry();
}

/**
 * TouchGFX application thread
 */
void ${moduleName}_Task(void *argument)
{
  // Calling farward to touchgfx_init in C++ domain
  touchgfx_taskEntry();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/