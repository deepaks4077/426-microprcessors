/*******************************************************************************
  * @file    Thread_Accelerometer.c
  * @author  Deepak Sharma
	* @version V1.0.0
  * @date    17-March-2016
  * @brief   This file inits the acc thread, configures the acc
	*					 and reads acc data
  ******************************************************************************
  */
	
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "configure.h"
#include "math.h"

extern osSemaphoreId semKeypad;
extern uint16_t Active_GPIO_Pin;
extern TIM_HandleTypeDef TIM_4;

extern int DISPLAY_ACC;
extern int DISPLAY_TEMP;

extern int ACC_PITCH;
extern int ACC_ROLL;

int keyNum = 0;

/* Private functions */
extern void rows();
extern void columns();
extern void configureKeypadTimer();

/* CMSIS_OS Thread related functions */
void initializeKeypadThread(void);
void Thread_Keypad(void const *argument);                 // thread function
osThreadId tid_Thread_Keypad;                              // thread id
osThreadDef(Thread_Keypad, osPriorityAboveNormal, 1, 0);

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_Keypad (void) {
  tid_Thread_Keypad = osThreadCreate(osThread(Thread_Keypad), NULL); // Start Keypad_thread
  if (!tid_Thread_Keypad) return(-1); 
  return(0);
}

 /*----------------------------------------------------------------------------
*      Thread  'Thread_Keypad': Reads and outputs Keypad data
 *---------------------------------------------------------------------------*/
void Thread_Keypad(void const *argument) {
	while(1){
		osSemaphoreWait(semKeypad,osWaitForever);
		
		rows();
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == 0){
				columns();
				if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)	{
					keyNum = 1;
				}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0){
					keyNum = 2;
				}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0){
					keyNum = 3;
				}
		}

		switch (keyNum){
			case 1:
				DISPLAY_TEMP=0;
				DISPLAY_ACC =1;
				ACC_PITCH = 0;
				ACC_ROLL = 1;
				break;
			case 2:
				DISPLAY_TEMP=0;
				DISPLAY_ACC =1;
				ACC_PITCH = 1;
				ACC_ROLL = 0;
				break;
			case 3:
				DISPLAY_TEMP=1;
				DISPLAY_ACC =0;
				ACC_PITCH = 1;
				ACC_ROLL = 0;
				break;
			default:
				break;
		}  	
	}
}

 /*---------------------------------------------------------------------------
*			Initialize GPIOE, LISD3SH
 *---------------------------------------------------------------------------*/
void initializeKeypadThread(void){
	configureKeypadTimer();
	rows();
}

/*----------------------------------------------------------------------------
 *      Initialize the GPIO associated with the Keypad interrupt (GPIOE0)
 *---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 *      Handler and Callback functions for interrupts
 *---------------------------------------------------------------------------*/
void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM_4);
}

/*----------------------------------------------------------------------------
 *      Utility functions for Keypad data (filtering and calibration-adjustments)
 *---------------------------------------------------------------------------*/
