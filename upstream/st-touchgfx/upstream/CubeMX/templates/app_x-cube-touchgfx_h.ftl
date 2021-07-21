[#ftl]
[#assign moduleName = "x-cube-gfx1"]
[#if ModuleName??]
	[#assign moduleName = ModuleName]
[/#if]
/**
  ******************************************************************************
  * File Name          : app_${moduleName?lower_case}.h
  ******************************************************************************
[@common.optinclude name=mxTmpFolder+"/license.tmp"/][#--include License text --]
  ******************************************************************************
  */

void ${fctName}(void);
void ${fctProcessName}(void);

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/