#include "user\RCC.h"

namespace rcc {

/******************************************************************************/
/* Private definitions ********************************************************/
/******************************************************************************/
#define DIVN1   480u
#define DIVP1   2u
#define DIVQ1   2u
#define DIVR1   2u
#define HSE_STARTUP_TIMEOUT   0x05000u

/******************************************************************************/
/* Exported functions *********************************************************/
/******************************************************************************/
void InitSystemClock() {
  uint32_t StartUpCounter = 0u;
  uint32_t HSEStatus = 0u;

  /* Select voltage scaling 1 */
  PWR->D3CR |= PWR_D3CR_VOS;
  /* Enable overdrive */
  SYSCFG->PWRCR |= SYSCFG_PWRCR_ODEN;
  /* Wait till Vos ready */
  while ((PWR->D3CR & PWR_D3CR_VOSRDY) != PWR_D3CR_VOSRDY) {
    ;
  }

  /* Enable HSE */
  RCC->CR |= RCC_CR_HSEON;
  /* Wait till HSE is ready and if timeout is reached exit */
  while ((HSEStatus == 0u) && (StartUpCounter < HSE_STARTUP_TIMEOUT)) {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    ++StartUpCounter;
  }

  if (HSEStatus == RCC_CR_HSERDY) {
    /* Set prescaler for PLL1 */
    RCC->PLLCKSELR &= ~RCC_PLLCKSELR_DIVM1;
    RCC->PLLCKSELR |= RCC_PLLCKSELR_DIVM1_2;

    /* Select HSE as PLL clock */
    RCC->PLLCKSELR |= RCC_PLLCKSELR_PLLSRC_HSE;

    /* Set PLL dividers */
    RCC->PLL1DIVR = DIVN1 - 1u;
    RCC->PLL1DIVR |= (DIVP1 - 1u) << RCC_PLL1DIVR_P1_Pos;
    RCC->PLL1DIVR |= (DIVQ1 - 1u) << RCC_PLL1DIVR_Q1_Pos;
    RCC->PLL1DIVR |= (DIVR1 - 1u) << RCC_PLL1DIVR_R1_Pos;

    /* Enable PLL divider output */
    RCC->PLLCFGR |= RCC_PLLCFGR_DIVP1EN;

    /* Enable PLL1 */
    RCC->CR |= RCC_CR_PLL1ON;

    /* Wait till PLL1 is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0u) {
      ;
    }

    /* Set flash latency */
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_4WS;

    /* Set D1 domain AHB prescaler */
    RCC->D1CFGR |= RCC_D1CFGR_HPRE_DIV2;

    /* Set D1 domain APB3 prescaler */
    RCC->D1CFGR |= RCC_D1CFGR_D1PPRE_DIV2;

    /* Set D2 domain APB1 prescaler */
    RCC->D2CFGR |= RCC_D2CFGR_D2PPRE1_DIV2;

    /* Set D2 domain APB2 prescaler */
    RCC->D2CFGR |= RCC_D2CFGR_D2PPRE2_DIV2;

    /* Set D3 domain APB4 prescaler */
    RCC->D3CFGR |= RCC_D3CFGR_D3PPRE_DIV2;

    /* Select PLL1 as system clock source */
    RCC->CFGR |= RCC_CFGR_SW_PLL1;
    /* Wait till PLL1 is used as system clock source */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL1) {
      ;
    }

    SystemCoreClockUpdate();
  } else {
    /* If HSE fails to start-up, the application will have wrong clock
    configuration. User can add here some code to deal with this error */
    while (1) {
      ;
    }
  }
}

/**
 * Get SYSCLK frequency
 * @return  SYSCLK frequency
 */
uint32_t GetSysclkFrequency() {
  return SystemCoreClock;
}

/**
 * Get HCLK frequency
 * @return  HCLK frequency
 */
uint32_t GetHclkFrequency() {
  return GetSysclkFrequency() >>
         ((D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_HPRE) >>
                            RCC_D1CFGR_HPRE_Pos]) & 0x1FU);
}

/**
 * Get PCLK1 frequency
 * @return  PCLK1 frequency
 */
uint32_t GetPclk1Frequency() {
  return GetHclkFrequency() >>
         ((D1CorePrescTable[(RCC->D2CFGR & RCC_D2CFGR_D2PPRE1) >>
                            RCC_D2CFGR_D2PPRE1_Pos]) & 0x1FU);
}

/**
 * Get PCLK2 frequency
 * @return  PCLK2 frequency
 */
uint32_t GetPclk2Frequency() {
  return GetHclkFrequency() >>
         ((D1CorePrescTable[(RCC->D2CFGR & RCC_D2CFGR_D2PPRE2) >>
                            RCC_D2CFGR_D2PPRE2_Pos]) & 0x1FU);
}

/**
 * Blocking delay
 * @param delay
 */
void StupidDelay(volatile uint32_t delay) {
  while (delay-- > 0u) {
    ;
  }
}

} /* namespace rcc */
