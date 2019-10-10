/* CMSIS */
#include "CMSIS\Device\stm32h7xx.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

/* User */
#include "user\RCC.h"

/******************************************************************************/
/* Private definitions ********************************************************/
/******************************************************************************/
#define LED_TOGGLE_DELAY    500u

/******************************************************************************/
/* Private function prototypes ************************************************/
/******************************************************************************/
static void InitAll();
static void InitHardware();
static void InitGpio();
static void InitRTOSObjects();
static void ToggleLed();

/******************************************************************************/
/* Tasks **********************************************************************/
/******************************************************************************/
void LedTask(void *pvParameters) {
  (void)pvParameters;

  while (1) {
    ToggleLed();
    vTaskDelay(LED_TOGGLE_DELAY);
  }
}

/******************************************************************************/
/* Main ***********************************************************************/
/******************************************************************************/
int main() {
  InitAll();
  vTaskStartScheduler();

  while (1) {
    ;
  }
}

/******************************************************************************/
/* Private functions **********************************************************/
/******************************************************************************/
static void InitAll() {
  InitHardware();
  InitRTOSObjects();
}

static void InitHardware() {
  rcc::InitSystemClock();
  InitGpio();
}

static void InitGpio() {
  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;  /* Enable GPIO clock */
  GPIOB->MODER &= ~GPIO_MODER_MODE14;   /* Clear MODER bits */
  GPIOB->MODER |= GPIO_MODER_MODE14_0;  /* GPIOB pin 14 in push-pull mode */
}

static void InitRTOSObjects() {
  /* Create tasks */
  xTaskCreate(LedTask, "", configMINIMAL_STACK_SIZE, NULL, 1u, NULL);
}

static void ToggleLed() {
  GPIOB->ODR ^= GPIO_ODR_OD14;          /* Toggle GPIOB pin 14 */
}
