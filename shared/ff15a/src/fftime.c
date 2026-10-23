#include "ff.h"

DWORD get_fattime(void)
{
  // Example: return a fixed timestamp (YYYY/MM/DD HH:MM:SS)
  // Format: bit31:25 Year from 1980 (0..127)
  //         bit24:21 Month (1..12)
  //         bit20:16 Day (1..31)
  //         bit15:11 Hour (0..23)
  //         bit10:5  Minute (0..59)
  //         bit4:0   Second / 2 (0..29)

  return ((DWORD)(2024 - 1980) << 25) // Year 2024
         | ((DWORD)4 << 21)           // Month April
         | ((DWORD)23 << 16)          // Day 23
         | ((DWORD)14 << 11)          // Hour 14
         | ((DWORD)30 << 5)           // Minute 30
         | ((DWORD)(0 / 2) << 0);     // Second 0
}
// Note: assumes hrtc handle is initalized!!
// DWORD get_fattime(void)
//{
//     RTC_TimeTypeDef time;
//     RTC_DateTypeDef date;
//     HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
//     HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
//
//     return  ((DWORD)(date.Year + 20) << 25)     // Year since 1980
//           | ((DWORD)date.Month << 21)
//           | ((DWORD)date.Date << 16)
//           | ((DWORD)time.Hours << 11)
//           | ((DWORD)time.Minutes << 5)
//           | ((DWORD)(time.Seconds / 2));
// }
