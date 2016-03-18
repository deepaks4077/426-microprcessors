/*******************************************************************************
  * @file    main.c
  * @author  Ashraf Suyyagh
	* @version V1.2.0
  * @date    17-January-2016
  * @brief   This file demonstrates flasing one LED at an interval of one second
	*          RTX based using CMSIS-RTOS 
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection
#include "lis3dsh.h"
#include "stm32f4xx_hal_gpio.h"
#include "math.h"

/* Private variables */
//float Pitch;
extern float DISPLAY_VALUE;

osSemaphoreId semTemperature;
osSemaphoreDef(semTemperature);

osSemaphoreId semSevenSegments;
osSemaphoreDef(semSevenSegments);

osSemaphoreId semAcc;
osSemaphoreDef(semAcc);

osSemaphoreId semKeypad;
osSemaphoreDef(semKeypad);

uint16_t Active_GPIO_Pin;

osSemaphoreId semDebounce;
osSemaphoreDef(semDebounce);

/* Private functions */
extern void initializeLED_IO			(void);
extern void start_Thread_LED			(void);
extern void Thread_LED						(void const *argument);
extern osThreadId tid_Thread_LED;

extern void initializeAccThread		(void);
extern void start_Thread_Acc			(void);
extern void Thread_Acc						(void const *argument);
extern osThreadId tid_Thread_Acc;

extern void initializeTemperatureThread		(void);
extern void start_Thread_Temperature			(void);
extern void Thread_Temperature						(void const *argument);
extern osThreadId tid_Thread_Temperature;

extern void initializeSevenSegmentsThread(void);
extern void Thread_SevenSegments(void const *argument);               
extern int start_Thread_SevenSegments(void);
extern osThreadId tid_Thread_SevenSegments;

extern void initializeKeypadThread(void);
extern void Thread_Keypad(void const *argument);               
extern int start_Thread_Keypad(void);
extern osThreadId tid_Thread_Keypad;

void initTemperatureSemaphore(void);
void initSevenSegmentSemaphore(void);
void initAccSemaphore(void);
void initKeypadSemaphore(void);

/**
	These lines are mandatory to make CMSIS-RTOS RTX work with te new Cube HAL
*/
#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif

/**
  * System Clock Configuration
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/**
  * Main function
  */
int main (void) {
	
	//printf("Hello");

  osKernelInitialize();                     /* initialize CMSIS-RTOS          */

  HAL_Init();                               /* Initialize the HAL Library     */

  SystemClock_Config();                     /* Configure the System Clock     */

	initTemperatureSemaphore();
	initSevenSegmentSemaphore();
	initAccSemaphore();
	semDebounce = osSemaphoreCreate(osSemaphore(semDebounce),0);
	
	/* User codes goes here*/
	initializeSevenSegmentsThread();
	start_Thread_SevenSegments();
	
	initializeAccThread();
	start_Thread_Acc();

	initializeTemperatureThread();
	start_Thread_Temperature();
	
	initializeKeypadThread();
	start_Thread_Keypad();
	
	initializeLED_IO();                       /* Initialize LED GPIO Buttons    */
  start_Thread_LED();                       /* Create LED thread              */
	/* User codes ends here*/
  
	osKernelStart();                          /* start thread execution         */
}

void initTemperatureSemaphore(void){
	semTemperature = osSemaphoreCreate(osSemaphore(semTemperature),0);
}

void initSevenSegmentSemaphore(void){
	semSevenSegments = osSemaphoreCreate(osSemaphore(semSevenSegments),0);
}

void initAccSemaphore(void){
	semAcc = osSemaphoreCreate(osSemaphore(semAcc),0);
}

void initKeypadSemaphore(void){
	semKeypad = osSemaphoreCreate(osSemaphore(semKeypad),0);
}

/*----------------------------------------------------------------------------
 *      Handler and Callback functions for interrupts
 *---------------------------------------------------------------------------*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM3){
		osSemaphoreRelease(semSevenSegments);
	}else if(htim->Instance == TIM2){
		osSemaphoreRelease(semTemperature);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_0){
		osSemaphoreRelease(semAcc);
	}else{
		Active_GPIO_Pin = GPIO_Pin;
		osSemaphoreRelease(semKeypad);
	}
}

/**
  * @brief  This function handles EXTIO (5-9) interrupt requests.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
		if(DEBOUNCE_FLAG == 0){
			// row 1
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == 0){
				// Delay to fix debouncing
				//osDelay(350);
				HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
			}	
			// row 2	
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) == 0){
				//osDelay(350);
				HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
			}
			// row 3
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0){
				//osDelay(350);
				HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
			}
			// row 4
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 0){
				//osDelay(350);
				HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
			}
		}
}


