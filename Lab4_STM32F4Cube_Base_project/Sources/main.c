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
float DISPLAY_VALUE;

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


/* These variables contol the state of the FSM*/
int DISPLAY_ACC = 1;
int DISPLAY_TEMP = 0;
extern int RAISE_THE_ALARM;
int ACC_PITCH = 1;
int ACC_ROLL = 0;
int toggle_on = 0;

extern float Pitch;
extern float Roll;
extern float Temperature;

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

extern void Reset_Display(void);

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

  osKernelInitialize();                     /* initialize CMSIS-RTOS          */

  HAL_Init();                               /* Initialize the HAL Library     */

  SystemClock_Config();                     /* Configure the System Clock     */

	initTemperatureSemaphore();
	initSevenSegmentSemaphore();
	initAccSemaphore();
	initKeypadSemaphore();
	
	/* User codes goes here*/
	initializeSevenSegmentsThread();
	start_Thread_SevenSegments();
	
	initializeAccThread();
	start_Thread_Acc();

	initializeTemperatureThread();
	start_Thread_Temperature();
	
	initializeKeypadThread();
	start_Thread_Keypad();
	
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
	
	if(htim->Instance == TIM4){
		osSemaphoreRelease(semKeypad);
	}else if(htim->Instance == TIM3){
		if (((toggle_on < 50) && RAISE_THE_ALARM == 1) || (RAISE_THE_ALARM == 0)){
			if(DISPLAY_ACC == 1){
				if (ACC_PITCH==1){
					DISPLAY_VALUE = Pitch;
				}else if (ACC_ROLL==1){
					DISPLAY_VALUE = Roll;
				}
			}else if(DISPLAY_TEMP == 1){
				DISPLAY_VALUE = Temperature;	
			}
			toggle_on++;
			osSemaphoreRelease(semSevenSegments);
		}else if ((toggle_on >= 50) && RAISE_THE_ALARM == 1){
			Reset_Display();
			toggle_on++;
			if (toggle_on > 100)
				toggle_on = 0;
		}
	}else if(htim->Instance == TIM2){
		osSemaphoreRelease(semTemperature);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_0){
		osSemaphoreRelease(semAcc);
	}
}


