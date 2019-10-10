/* CMSIS */
#include "CMSIS\Device\stm32h7xx.h"

/******************************************************************************/
/* Private definitions ********************************************************/
/******************************************************************************/
#define LED_TOGGLE_DELAY  1000000u

/******************************************************************************/
/* Private function prototypes ************************************************/
/******************************************************************************/
static void InitGpio();
static void ToggleLed();
static void DummyDelay(const int delay);

/******************************************************************************/
/* Main ***********************************************************************/
/******************************************************************************/
int main() {
  InitGpio();

  while (1) {
    ToggleLed();
    DummyDelay(LED_TOGGLE_DELAY);
  }
}

/******************************************************************************/
/* Private functions **********************************************************/
/******************************************************************************/
static void InitGpio() {
  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;  /* Enable GPIO clock */
  GPIOB->MODER &= ~GPIO_MODER_MODE14;   /* Clear MODER bits */
  GPIOB->MODER |= GPIO_MODER_MODE14_0;  /* GPIOB pin 14 in push-pull mode */
}

static void ToggleLed() {
  GPIOB->ODR ^= GPIO_ODR_OD14;          /* Toggle GPIOB pin 14 */
}

static void DummyDelay(const int delay) {
  for (volatile int i = 0; i < delay; ++i) {
    ;
  }
}
