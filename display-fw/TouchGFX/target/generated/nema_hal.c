
/**
  ******************************************************************************
  * @file    nema_hal.c
  * @author  MCD Application Team
  * @brief   NemaGFX Interfaces and Platform Specific APIs with FreeRTOS CMSISV2
  *          support.
  *          This file provides NemaGFX functions to manage the following
  *          functionalities of the NemaGFX library:
  *           + Initialization and de-initialization functions
  *           + GPU2D Registers access operation functions
  *           + Buffer Creation, Destruction, MMAP and Flusing functions
  *           + Interrupt management functions
  *           + Memory management functions
  *           + Locking/Unlocking resources functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#include <touchgfx/hal/Config.hpp>
#include <nema_sys_defs.h>
#include <nema_core.h>

#include <assert.h>
#include <string.h>

#include <stm32u5xx_hal.h>

#include <cmsis_os2.h>

#include "tsi_malloc.h"

#define RING_SIZE                      1024 /* Ring Buffer Size in byte */
#define NEMAGFX_MEM_POOL_SIZE          10240 /* NemaGFX byte pool size in byte */

LOCATION_PRAGMA_NOLOAD("Nemagfx_Memory_Pool_Buffer")
static uint8_t nemagfx_pool_mem[NEMAGFX_MEM_POOL_SIZE] LOCATION_ATTRIBUTE_NOLOAD("Nemagfx_Memory_Pool_Buffer"); /* NemaGFX memory pool */

static nema_ringbuffer_t ring_buffer_str;
volatile static int last_cl_id = -1;
extern GPU2D_HandleTypeDef hgpu2d;

static osSemaphoreId_t nema_irq_sem = NULL; // Declare CL IRQ semaphore

#if (USE_HAL_GPU2D_REGISTER_CALLBACKS == 1)
static void GPU2D_CommandListCpltCallback(GPU2D_HandleTypeDef* hgpu2d, uint32_t CmdListID)
#else /* USE_HAL_GPU2D_REGISTER_CALLBACKS = 0 */
void HAL_GPU2D_CommandListCpltCallback(GPU2D_HandleTypeDef* hgpu2d, uint32_t CmdListID)
#endif /* USE_HAL_GPU2D_REGISTER_CALLBACKS = 1 */
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hgpu2d);

    last_cl_id = CmdListID;

    /* Return a token back to a semaphore */
    osSemaphoreRelease(nema_irq_sem);
}

int32_t nema_sys_init(void)
{
    int error_code = 0;

    /* Setup GPU2D Callback */
#if (USE_HAL_GPU2D_REGISTER_CALLBACKS == 1)
    /* Register Command List Comlete Callback */
    HAL_GPU2D_RegisterCommandListCpltCallback(&hgpu2d, GPU2D_CommandListCpltCallback);
#endif /* USE_HAL_GPU2D_REGISTER_CALLBACKS = 1 */

    /* Create IRQ semaphore */
    nema_irq_sem = osSemaphoreNew(1, 1, NULL);
    assert(nema_irq_sem != NULL);

    /* Initialise Mem Space */
    error_code = tsi_malloc_init_pool_aligned(0, (void*)nemagfx_pool_mem, (uintptr_t)nemagfx_pool_mem, NEMAGFX_MEM_POOL_SIZE, 1, 8);
    assert(error_code == 0);

    /* Allocate ring_buffer memory */
    ring_buffer_str.bo = nema_buffer_create(RING_SIZE);
    assert(ring_buffer_str.bo.base_virt);

    /* Initialize Ring Buffer */
    error_code = nema_rb_init(&ring_buffer_str, 1);
    if (error_code < 0)
    {
        return error_code;
    }

    /* Reset last_cl_id counter */
    last_cl_id = 0;

    return error_code;
}

uint32_t nema_reg_read(uint32_t reg)
{
    return HAL_GPU2D_ReadRegister(&hgpu2d, reg);
}

void nema_reg_write(uint32_t reg, uint32_t value)
{
    HAL_GPU2D_WriteRegister(&hgpu2d, reg, value);
}

int nema_wait_irq(void)
{
    /* Wait indefinitely for a free semaphore */
    osSemaphoreAcquire(nema_irq_sem, osWaitForever);

    return 0;
}

int nema_wait_irq_cl(int cl_id)
{
    while (last_cl_id < cl_id)
    {
        (void)nema_wait_irq();
    }

    return 0;
}

int nema_wait_irq_brk(int brk_id)
{
    while (nema_reg_read(GPU2D_BREAKPOINT) == 0U)
    {
        (void)nema_wait_irq();
    }

    return 0;
}

void nema_host_free(void* ptr)
{
    tsi_free(ptr);
}

void* nema_host_malloc(unsigned size)
{
    return tsi_malloc(size);
}

nema_buffer_t nema_buffer_create(int size)
{
    nema_buffer_t bo;
    memset(&bo, 0, sizeof(bo));
    bo.base_virt = tsi_malloc(size);
    bo.base_phys = (uint32_t)bo.base_virt;
    bo.size      = size;
    assert(bo.base_virt != 0 && "Unable to allocate memory in nema_buffer_create");

    return bo;
}

nema_buffer_t nema_buffer_create_pool(int pool, int size)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(pool);

    return nema_buffer_create(size);
}

void* nema_buffer_map(nema_buffer_t* bo)
{
    return bo->base_virt;
}

void nema_buffer_unmap(nema_buffer_t* bo)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(bo);
}

void nema_buffer_destroy(nema_buffer_t* bo)
{
    if (bo->fd == -1)
    {
        return; /* Buffer weren't allocated! */
    }

    tsi_free(bo->base_virt);

    bo->base_virt = (void*)0;
    bo->base_phys = 0;
    bo->size      = 0;
    bo->fd        = -1; /* Buffer not allocated */
}

uintptr_t nema_buffer_phys(nema_buffer_t* bo)
{
    return bo->base_phys;
}

void nema_buffer_flush(nema_buffer_t* bo)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(bo);
}

int nema_mutex_lock(int mutex_id)
{
    int retval = 0;

    /* USER CODE BEGIN nema_mutex_lock */
    /* Prevent unused argument(s) compilation warning */
    UNUSED(mutex_id);
    /* USER CODE END nema_mutex_lock */

    return retval;
}

int nema_mutex_unlock(int mutex_id)
{
    int retval = 0;

    /* USER CODE BEGIN nema_mutex_unlock */
    /* Prevent unused argument(s) compilation warning */
    UNUSED(mutex_id);
    /* USER CODE END nema_mutex_unlock */

    return retval;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
