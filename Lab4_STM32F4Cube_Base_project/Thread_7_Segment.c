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
#include "segments.h"
#define ALARM_THRESHOLD 								27

extern void configTimer(void);
extern void configSegmentPins(void);
extern void configureSevenSegmentTimer(void); 

/* Private variables */
extern float Pitch;
extern float Roll;
extern float Temperature;

extern TIM_HandleTypeDef TIM_3;
extern osSemaphoreId semSevenSegments;

float first_digit, second_digit, third_digit;
int decimal_pos = -1;
int TOGGLE_PIN = 0;

int DISPLAY_CTR = 0; 														// Controls the rate at which a new value is displayed
int UPDATE_DIGIT_CTR = 0; 											// Controls the digit to display on the LED
float DISPLAY_VALUE = 0.0;

/* Private functions */
void configureTIM3Timer(void);
void configureSegmentPins(void);

/**
  * @brief  This function updates the digits that need to be displayed on the 7-Segment display
  * @param  [float] 		The angle to get the LED digits from
	* @retval [none]					
  */
void getNewValue(float value);

/**
  * @brief  This function handles the logic to display values on the 7-Segment display
  * @param  [void] 
	* @retval [none]					
  */
void displayValue(void);

/* CMSIS_OS Thread related functions */
void initializeSevenSegmentsThread(void);
void Thread_SevenSegments(void const *argument);                 // thread function
osThreadId tid_Thread_SevenSegments;                              // thread id
osThreadDef(Thread_SevenSegments, osPriorityNormal, 1, 0);

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_SevenSegments(void) {
  tid_Thread_SevenSegments = osThreadCreate(osThread(Thread_SevenSegments), NULL); // Start Acc_thread
  if (!tid_Thread_SevenSegments) return(-1); 
  return(0);
}

 /*----------------------------------------------------------------------------
*      Thread  'Thread_Acc': Reads and outputs Acc data
 *---------------------------------------------------------------------------*/
void Thread_SevenSegments(void const *argument) {
	while(1){
		osSemaphoreWait(semSevenSegments,osWaitForever);
		if(Temperature > ALARM_THRESHOLD){
			osDelay(10);
			if(TOGGLE_PIN == 0){
				Reset_Display();
				TOGGLE_PIN = 1;
			}else{
				getNewValue(Pitch);
				displayValue();
				TOGGLE_PIN = 0;
			}
		}else{
			getNewValue(Pitch);
			displayValue();
		}
	}
}

 /*---------------------------------------------------------------------------
*			Initialize GPIOE, LISD3SH
 *---------------------------------------------------------------------------*/
void initializeSevenSegmentsThread(void){
	configureSegmentPins();
	configureTIM3Timer();
}

/*----------------------------------------------------------------------------
 *      Initialize the GPIO associated with the Acc interrupt (GPIOE0)
 *---------------------------------------------------------------------------*/
void configureSegmentPins(void){
	configSegmentPins();
}

void configureTIM3Timer(void){
	configureSevenSegmentTimer();
}
	
/*----------------------------------------------------------------------------
 *      Handler and Callback functions for TIM3 interrupt
 *---------------------------------------------------------------------------*/
/**
  * @brief  This function handles TIM3 interrupts
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void){
	DISPLAY_CTR++;
	HAL_TIM_IRQHandler(&TIM_3);
}

/*----------------------------------------------------------------------------
 *      Utility functions for Acc data (filtering and calibration-adjustments)
 *---------------------------------------------------------------------------*/
void getNewValue(float value){
	
	value = fabsf(value);
	if(value < 10){
			first_digit = Get_Digit_In_Place(value*10,1);
			second_digit = Get_Digit_In_Place(value*10,10);
			third_digit = Get_Digit_In_Place(value*10,100);
	}else if(value < 100){
			first_digit = Get_Digit_In_Place(value,1);
			second_digit = Get_Digit_In_Place(value,10);
			third_digit = Get_Digit_In_Place(value,100);
	}else{
			first_digit = Get_Digit_In_Place(value/10.0,1);
			second_digit = Get_Digit_In_Place(value/10.0,10);
			third_digit = Get_Digit_In_Place(value/10.0,100);
	}
	
	DISPLAY_VALUE = value;
}

void displayValue(void){
	if(UPDATE_DIGIT_CTR == 0){
		
		if(DISPLAY_VALUE< 10){
			decimal_pos = 1;
		}else if(DISPLAY_VALUE< 100){
			decimal_pos = 2;
		}else{
			decimal_pos = 3;
		}
		
		Reset_Display();
		Display_Digit_At_Pos(1,third_digit);
		
		if(decimal_pos == 1){
			AddDecimal();
		}
		
		UPDATE_DIGIT_CTR++;
	}else if(UPDATE_DIGIT_CTR == 1){
		Reset_Display();
		Display_Digit_At_Pos(2,second_digit);
		
		if(decimal_pos == 2){
			AddDecimal();
		}
		
		UPDATE_DIGIT_CTR++;
	}else{
		Reset_Display();
		Display_Digit_At_Pos(3,first_digit);
		UPDATE_DIGIT_CTR = 0;
	}
}