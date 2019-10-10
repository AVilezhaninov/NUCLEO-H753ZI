#ifndef RCC_H_
#define RCC_H_

/* CMSIS */
#include "CMSIS\Device\stm32h7xx.h"
#include "CMSIS\Device\system_stm32h7xx.h"

namespace rcc {

/******************************************************************************/
/* Exported functions prototypes **********************************************/
/******************************************************************************/
void InitSystemClock();

uint32_t GetSysclkFrequency();
uint32_t GetHclkFrequency();
uint32_t GetPclk1Frequency();
uint32_t GetPclk2Frequency();

void StupidDelay(volatile uint32_t delay);

} /* namespace rcc */

#endif /* RCC_H_ */
