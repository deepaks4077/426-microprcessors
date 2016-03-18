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
int goal = 0;
int input = 0;

/* Private functions */
extern void rows();
extern void columns();
void configureKeypad(void);

/* CMSIS_OS Thread related functions */
void initializeKeypadThread(void);
void Thread_Keypad(void const *argument);                 // thread function
osThreadId tid_Thread_Keypad;                              // thread id
osThreadDef(Thread_Keypad, osPriorityNormal, 1, 0);

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
		if((goal<180) && input == 0){
			if (Active_GPIO_Pin == GPIO_PIN_6){ 
				// initiate the columns to set them as input to read the bit 			
				columns();																							
				// row one column 1
				if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0){
					///printf("1");
					goal = 1 + (goal*10);
				}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0){ // row one column 2
					//printf("2");
					goal = 2 + (goal*10);
				}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0){ // row one column 3
					//printf("3");
					goal = 3 + (goal*10);
				}
				// start the rows again for the next button press
				rows();												
			}	else if (Active_GPIO_Pin == GPIO_PIN_7){
				columns();
				// row 2 column 1
				if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0){
					//printf("4");
					goal = 4 + (goal*10);

				}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0){ // row 2 column 2 
					//printf("5");
					goal = 5 + (goal*10);

				}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0){ // row 2 column 3
					//printf("6");
					goal = 6 + (goal*10);

				}
			
				rows();
			}	
			else if (Active_GPIO_Pin == GPIO_PIN_8){
				columns();
				// row 3 column 1 
				if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0){
					//printf("7");
					goal = 7 + (goal*10);

				}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0){ // row 3 column 2
					//printf("8");
					goal = 8 + (goal*10);

				}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0){ // row 3 column 3
					//printf("9");
					goal = 9 + (goal*10);

				}

				rows();
			}	
			else if (Active_GPIO_Pin == GPIO_PIN_9){
				columns();
				// row 4 column 1 
				if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0){
					goal = 0;

				}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0){ // row 4 column 2 
					//printf("0");
					goal = 0 + (goal*10);

				}
				// row 4 column 3
				
				/*
					Once the hash button is presed, get out of this "if" statement and stop checking for button 
					presses and move to the direction function.
					
					Clear all interrupts and disable so that we dont have to check if a row is pressed again.
				*/	
				else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0){
					input = 1;
					//printf("Changed mode, GOAL = %d\n", goal);
					HAL_NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
					HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
				}
				
				if(input == 0){
					// Added to handle a '0' input to the last row
					rows();
				}
			}	
		}
	}
}

 /*---------------------------------------------------------------------------
*			Initialize GPIOE, LISD3SH
 *---------------------------------------------------------------------------*/
void initializeKeypadThread(void){
	configureKeypad();
}

/*----------------------------------------------------------------------------
 *      Initialize the GPIO associated with the Keypad interrupt (GPIOE0)
 *---------------------------------------------------------------------------*/

void configureKeypad(void){
	rows();
}


/*----------------------------------------------------------------------------
 *      Handler and Callback functions for interrupts
 *---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 *      Utility functions for Keypad data (filtering and calibration-adjustments)
 *---------------------------------------------------------------------------*/
