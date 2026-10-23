/*
******************************************************************************
**
**  File        : LinkerScript.ld
**
**  Author      : STM32CubeIDE
**
**  Abstract    : Linker script for STM32U5A9xJ Device from STM32U5 series
**                      4096Kbytes FLASH
**                      2512Kbytes RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed as is without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** Copyright (c) 2023 STMicroelectronics.
** All rights reserved.
**
** This software is licensed under terms that can be found in the LICENSE file
** in the root directory of this software component.
** If no LICENSE file comes with this software, it is provided AS-IS.
**
*****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM); /* end of "RAM" Ram type memory */

_Min_Heap_Size = 0x1000; /* required amount of heap */
_Min_Stack_Size = 0x1000; /* required amount of stack */

/* The first 256 KiB belong to display-bootloader, which checks the image header
 * and jumps to the vector table at 0x08040400 (BOOTLOADER_ISR_OFFSET) */
_Bootloader_Size = 256K;

/* The last 8 KiB page holds the user config (menuView::Settings) */
_Config_Size = 8K;

/* Memories definition */
MEMORY
{
  FB_RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 2250K /*double 24bit buffer 800x480*/
  RAM (xrw)   : ORIGIN = 0x20232800, LENGTH = 246K
  SRAM4 (xrw) : ORIGIN = 0x28000000, LENGTH = 16K
  FLASH (rx)  : ORIGIN = 0x08000000 + _Bootloader_Size, LENGTH = 4096K - _Bootloader_Size - _Config_Size
  CONFIG_FLASH (rx) : ORIGIN = 0x08000000 + 4096K - _Config_Size, LENGTH = _Config_Size
  OSPI (rw)	  : ORIGIN = 0x90000000, LENGTH = 64M
}

/* Sections */
SECTIONS
{
  /* Image header (Modules/image_header/src/image_header.s), filled in by
   * tools/imgtool.py. Must be first in the user flash. */
  .image_header :
  {
    KEEP(*(.image_header))
  } >FLASH

  /* The startup code into "FLASH" Rom type memory. VTOR needs the vector table
   * aligned to its size rounded up to a power of two (620 bytes -> 1 KiB). */
  .isr_vector :
  {
    . = ALIGN(0x400);
    KEEP(*(.isr_vector)) /* Startup code */
  } >FLASH

  /* place the user config area in the reserved CONFIG_FLASH region */
  .user_config (NOLOAD) :
  {
    . = ORIGIN(CONFIG_FLASH);            /* force location to start of CONFIG_FLASH page */
    . = ALIGN(16);                       /* align to 16 bytes (quadword) */
    PROVIDE( _user_config_start = . );
    KEEP(*(.user_config))                /* keep any data placed in .user_config */
    . = . + 16;                          /* reserve exactly 16 bytes */
    PROVIDE( _user_config_end = . );
    . = ALIGN(4);
  } >CONFIG_FLASH

  /* The program code and other data into "FLASH" Rom type memory */
  .text :
  {
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    _etext = .;        /* define a global symbols at end of code */
  } >FLASH

  /* Constant data into "FLASH" Rom type memory */
  .rodata :
  {
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  } >FLASH

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } >FLASH

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  /* Used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections into "RAM" Ram type memory */
  .data :
  {
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    *(.RamFunc)        /* .RamFunc sections */
    *(.RamFunc*)       /* .RamFunc* sections */

    _edata = .;        /* define a global symbol at data end */
  } >RAM AT> FLASH

  /* Uninitialized data section into "RAM" Ram type memory */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM

  /* User_heap_stack section, used to check that there is enough "RAM" Ram type memory left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM

  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
  
FramebufferSection (NOLOAD) :
  {
    *(TouchGFX_Framebuffer TouchGFX_Framebuffer.*)
    *(.gnu.linkonce.r.*)
    . = ALIGN(0x4);
  } >FB_RAM
  
ExtFlashSection :
  {
    *(ExtFlashSection ExtFlashSection.*)
    *(.gnu.linkonce.r.*)
    . = ALIGN(0x4);
  } >OSPI
  
}
