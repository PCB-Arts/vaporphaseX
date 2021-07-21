[#ftl]
[#list SWIPdatas as data]
[#assign params = data.parameters]
/**
  ******************************************************************************
  * File Name          : ${name.split("/")?last}
  ******************************************************************************
[@common.optinclude name=mxTmpFolder+"/license.tmp"/][#--include License text --]
  ******************************************************************************
  */
[#if params.tgfx_oswrapper == "NoOS"]
#include <${FamilyName?lower_case}xx_hal.h>
#include <TouchGFXHAL.hpp>
#include <touchgfx/hal/OSWrappers.hpp>
#n
static volatile uint32_t fb_sem;
static volatile uint32_t vsync_sem;
#n
using namespace touchgfx;
#n
/*
 * Initialize frame buffer semaphore and queue/mutex for VSYNC signal.
 */
void OSWrappers::initialize()
{
  fb_sem = 0;
  vsync_sem = 0;
}

/*
 * Take the frame buffer semaphore. Blocks until semaphore is available.
 */
void OSWrappers::takeFrameBufferSemaphore()
{
  while(fb_sem);
  fb_sem = 1;
}

/*
 * Release the frame buffer semaphore.
 */
void OSWrappers::giveFrameBufferSemaphore()
{
  fb_sem = 0;
}

/*
 * Attempt to obtain the frame buffer semaphore. If semaphore is not available, do
 * nothing.
 *
 * Note must return immediately! This function does not care who has the taken the semaphore,
 * it only serves to make sure that the semaphore is taken by someone.
 */
void OSWrappers::tryTakeFrameBufferSemaphore()
{
  fb_sem = 1;
}

/*
 * Release the frame buffer semaphore in a way that is safe in interrupt context. Called
 * from ISR.
 *
 * Release the frame buffer semaphore in a way that is safe in interrupt context.
 * Called from ISR.
 */
void OSWrappers::giveFrameBufferSemaphoreFromISR()
{
  fb_sem = 0;
}

/*
 * Signal that a VSYNC has occurred. Should make the vsync queue/mutex available.
 *
 * Note This function is called from an ISR, and should (depending on OS) trigger a
 * scheduling.
 */
void OSWrappers::signalVSync()
{
  vsync_sem = 1;
}

/*
  * Signal that the rendering of the frame has completed. Used by
  * some systems to avoid using any previous vsync.
  */
void OSWrappers::signalRenderingDone()
{
    vsync_sem = 0;
}

/*
 * This function checks if a VSync occurred after last rendering.
 * The function is used in systems that cannot wait in  waitForVSync
 * (because they are also checking other event sources.
 *
 * @note signalRenderingDone is typically used together with this function.
 *
 * @return True if VSync occurred.
 */
bool OSWrappers::isVSyncAvailable()
{
  return vsync_sem;
}

/*
 * This function check if a VSYNC has occured.
 * If VSYNC has occured, signal TouchGFX to start a rendering
 */
void OSWrappers::waitForVSync()
{
  if(vsync_sem)
  {
    vsync_sem = 0;
    HAL::getInstance()->backPorchExited();
  }
}

/*
 * A function that causes executing task to sleep for a number of milliseconds.
 *
 * A function that causes executing task to sleep for a number of milliseconds.
 * This function is OPTIONAL. It is only used by the TouchGFX in the case of
 * a specific frame refresh strategy (REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL).
 * Due to backwards compatibility, in order for this function to be useable by the HAL
 * the function must be explicitly registered:
 * hal.registerTaskDelayFunction(&OSWrappers::taskDelay)
 *
 * see HAL::setFrameRefreshStrategy(FrameRefreshStrategy s)
 * see HAL::registerTaskDelayFunction(void (*delayF)(uint16_t))
 */
void OSWrappers::taskDelay(uint16_t ms)
{
    HAL_Delay(ms);
}
[/#if]
[#if params.tgfx_oswrapper == "CMSIS V1"]
#include <cassert>
#include <cmsis_os.h>
#include <touchgfx/hal/GPIO.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/OSWrappers.hpp>

static osSemaphoreId frame_buffer_sem;      // Semaphore ID
osSemaphoreDef(frame_buffer_sem);           // Semaphore definition

static osSemaphoreId vsync_sem;             // Semaphore ID
osSemaphoreDef(vsync_sem);                  // Semaphore definition

using namespace touchgfx;

/*
 * Initialize frame buffer semaphore and queue/mutex for VSYNC signal.
 */
void OSWrappers::initialize()
{
    frame_buffer_sem = osSemaphoreCreate(osSemaphore(frame_buffer_sem), 1);
    assert((frame_buffer_sem != NULL) && "Creation of framebuffer semaphore failed");

    vsync_sem = osSemaphoreCreate(osSemaphore(vsync_sem), 1);
    assert((vsync_sem != NULL) && "Creation of vsync semaphore failed");
}

/*
 * Take the frame buffer semaphore. Blocks until semaphore is available.
 */
void OSWrappers::takeFrameBufferSemaphore()
{
    osSemaphoreWait(frame_buffer_sem, osWaitForever);
}

/*
 * Release the frame buffer semaphore.
 */
void OSWrappers::giveFrameBufferSemaphore()
{
    osSemaphoreRelease(frame_buffer_sem);
}

/*
 * Attempt to obtain the frame buffer semaphore. If semaphore is not available, do
 * nothing.
 *
 * Note must return immediately! This function does not care who has the taken the semaphore,
 * it only serves to make sure that the semaphore is taken by someone.
 */
void OSWrappers::tryTakeFrameBufferSemaphore()
{
    osSemaphoreWait(frame_buffer_sem, 0);
}

/*
 * Release the frame buffer semaphore in a way that is safe in interrupt context. Called
 * from ISR.
 *
 * Release the frame buffer semaphore in a way that is safe in interrupt context.
 * Called from ISR.
 */
void OSWrappers::giveFrameBufferSemaphoreFromISR()
{
    // Release of semaphore inside an interrupt is handled by the CMSIS layer
    osSemaphoreRelease(frame_buffer_sem);
}

/*
 * Signal that a VSYNC has occurred. Should make the vsync queue/mutex available.
 *
 * Note This function is called from an ISR, and should (depending on OS) trigger a
 * scheduling.
 */
void OSWrappers::signalVSync()
{
    // Release of semaphore inside an interrupt is handled by the CMSIS layer
    osSemaphoreRelease(vsync_sem);
}

/*
 * This function blocks until a VSYNC occurs.
 *
 * Note This function must first clear the mutex/queue and then wait for the next one to
 * occur.
 */
void OSWrappers::waitForVSync()
{
    // First make sure the queue is empty, by trying to remove an element with 0 timeout.
    osSemaphoreWait(vsync_sem, 0);

    // Then, wait for next VSYNC to occur.
    osSemaphoreWait(vsync_sem, osWaitForever);
}

/*
 * A function that causes executing task to sleep for a number of milliseconds.
 *
 * A function that causes executing task to sleep for a number of milliseconds.
 * This function is OPTIONAL. It is only used by the TouchGFX in the case of
 * a specific frame refresh strategy (REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL).
 * Due to backwards compatibility, in order for this function to be useable by the HAL
 * the function must be explicitly registered:
 * hal.registerTaskDelayFunction(&OSWrappers::taskDelay)
 *
 * see HAL::setFrameRefreshStrategy(FrameRefreshStrategy s)
 * see HAL::registerTaskDelayFunction(void (*delayF)(uint16_t))
 */
void OSWrappers::taskDelay(uint16_t ms)
{
    osDelay(static_cast<uint32_t>(ms));
}
[/#if]
[#if params.tgfx_oswrapper == "CMSIS V2"]
#include <cassert>
#include <cmsis_os2.h>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/OSWrappers.hpp>

static osSemaphoreId_t frame_buffer_sem = NULL;
static osMessageQueueId_t vsync_queue = NULL;

// Just a dummy value to insert in the VSYNC queue.
static uint32_t dummy = 0x5a;
#n
using namespace touchgfx;
#n
/*
 * Initialize frame buffer semaphore and queue/mutex for VSYNC signal.
 */
void OSWrappers::initialize()
{
    // Create a queue of length 1
    frame_buffer_sem = osSemaphoreNew(1, 1, NULL); // Binary semaphore
    assert((frame_buffer_sem != NULL) && "Creation of framebuffer semaphore failed");

    // Create a queue of length 1
    vsync_queue = osMessageQueueNew(1, 4, NULL);
    assert((vsync_queue != NULL) && "Creation of vsync message queue failed");
}

/*
 * Take the frame buffer semaphore. Blocks until semaphore is available.
 */
void OSWrappers::takeFrameBufferSemaphore()
{
    osSemaphoreAcquire(frame_buffer_sem, osWaitForever);
}

/*
 * Release the frame buffer semaphore.
 */
void OSWrappers::giveFrameBufferSemaphore()
{
    osSemaphoreRelease(frame_buffer_sem);
}

/*
 * Attempt to obtain the frame buffer semaphore. If semaphore is not available, do
 * nothing.
 *
 * Note must return immediately! This function does not care who has the taken the semaphore,
 * it only serves to make sure that the semaphore is taken by someone.
 */
void OSWrappers::tryTakeFrameBufferSemaphore()
{
    osSemaphoreAcquire(frame_buffer_sem, 0);
}

/*
 * Release the frame buffer semaphore in a way that is safe in interrupt context. Called
 * from ISR.
 *
 * Release the frame buffer semaphore in a way that is safe in interrupt context.
 * Called from ISR.
 */
void OSWrappers::giveFrameBufferSemaphoreFromISR()
{
    osSemaphoreRelease(frame_buffer_sem);
}

/*
 * Signal that a VSYNC has occurred. Should make the vsync queue/mutex available.
 *
 * Note This function is called from an ISR, and should (depending on OS) trigger a
 * scheduling.
 */
void OSWrappers::signalVSync()
{
    osMessageQueuePut(vsync_queue, &dummy, 0, 0);
}

/*
 * This function blocks until a VSYNC occurs.
 *
 * Note This function must first clear the mutex/queue and then wait for the next one to
 * occur.
 */
void OSWrappers::waitForVSync()
{
    uint32_t dummyGet;
    // First make sure the queue is empty, by trying to remove an element with 0 timeout.
    osMessageQueueGet(vsync_queue, &dummyGet, 0, 0);

    // Then, wait for next VSYNC to occur.
    osMessageQueueGet(vsync_queue, &dummyGet, 0, osWaitForever);
}

/*
 * A function that causes executing task to sleep for a number of milliseconds.
 *
 * A function that causes executing task to sleep for a number of milliseconds.
 * This function is OPTIONAL. It is only used by the TouchGFX in the case of
 * a specific frame refresh strategy (REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL).
 * Due to backwards compatibility, in order for this function to be useable by the HAL
 * the function must be explicitly registered:
 * hal.registerTaskDelayFunction(&OSWrappers::taskDelay)
 *
 * see HAL::setFrameRefreshStrategy(FrameRefreshStrategy s)
 * see HAL::registerTaskDelayFunction(void (*delayF)(uint16_t))
 */
void OSWrappers::taskDelay(uint16_t ms)
{
    osDelay(static_cast<uint32_t>(ms));
}
[/#if]
[#if params.tgfx_oswrapper == "Custom"]
#n
/* USER CODE BEGIN ${name} */
#n
#include <cassert>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/OSWrappers.hpp>
using namespace touchgfx;
#n
/*
 * Initialize frame buffer semaphore and queue/mutex for VSYNC signal.
 */
void OSWrappers::initialize()
{

}

/*
 * Take the frame buffer semaphore. Blocks until semaphore is available.
 */
void OSWrappers::takeFrameBufferSemaphore()
{

}

/*
 * Release the frame buffer semaphore.
 */
void OSWrappers::giveFrameBufferSemaphore()
{

}

/*
 * Attempt to obtain the frame buffer semaphore. If semaphore is not available, do
 * nothing.
 *
 * Note must return immediately! This function does not care who has the taken the semaphore,
 * it only serves to make sure that the semaphore is taken by someone.
 */
void OSWrappers::tryTakeFrameBufferSemaphore()
{

}

/*
 * Release the frame buffer semaphore in a way that is safe in interrupt context. Called
 * from ISR.
 *
 * Release the frame buffer semaphore in a way that is safe in interrupt context.
 * Called from ISR.
 */
void OSWrappers::giveFrameBufferSemaphoreFromISR()
{

}

/*
 * Signal that a VSYNC has occurred. Should make the vsync queue/mutex available.
 *
 * Note This function is called from an ISR, and should (depending on OS) trigger a
 * scheduling.
 */
void OSWrappers::signalVSync()
{

}

/*
 * This function blocks until a VSYNC occurs.
 *
 * Note This function must first clear the mutex/queue and then wait for the next one to
 * occur.
 */
void OSWrappers::waitForVSync()
{

}

/*
 * A function that causes executing task to sleep for a number of milliseconds.
 *
 * A function that causes executing task to sleep for a number of milliseconds.
 * This function is OPTIONAL. It is only used by the TouchGFX in the case of
 * a specific frame refresh strategy (REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL).
 * Due to backwards compatibility, in order for this function to be useable by the HAL
 * the function must be explicitly registered:
 * hal.registerTaskDelayFunction(&OSWrappers::taskDelay)
 *
 * see HAL::setFrameRefreshStrategy(FrameRefreshStrategy s)
 * see HAL::registerTaskDelayFunction(void (*delayF)(uint16_t))
 */
void OSWrappers::taskDelay(uint16_t ms)
{

}

/* USER CODE END ${name} */
[/#if]
[/#list]

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
