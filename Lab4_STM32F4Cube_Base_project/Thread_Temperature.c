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
#include "kalman_filter.h"
#define ALARM_THRESHOLD 								29

extern void configADC(void);
extern void configureTemperatureTimer(void);
extern ADC_HandleTypeDef ADC1_Handle;
extern osSemaphoreId semTemperature;
extern TIM_HandleTypeDef TIM_2;
int RAISE_THE_ALARM = 0;
float Temperature;
kalman_state kstate;

/* Private functions */
void configureTemperature(void);
float Convert_Voltage_To_Temperature(uint32_t Voltage);

/* CMSIS_OS Thread related functions */
void initializeTemperatureThread(void);
void Thread_Temperature(void const *argument);                 // thread function
osThreadId tid_Thread_Temperature;                              // thread id
osThreadDef(Thread_Temperature, osPriorityNormal, 1, 0);

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_Temperature (void) {
  tid_Thread_Temperature = osThreadCreate(osThread(Thread_Temperature), NULL); // Start Acc_thread
  if (!tid_Thread_Temperature) return(-1); 
  return(0);
}

/*----------------------------------------------------------------------------
 *      Thread  'Thread_Acc': Reads and outputs Acc data
 *---------------------------------------------------------------------------*/
void Thread_Temperature(void const *argument) {
	float Voltage;
	while(1){
		osSemaphoreWait(semTemperature,osWaitForever);
		Voltage = HAL_ADC_GetValue(&ADC1_Handle);
		Voltage = kalmanFilter(Voltage,&kstate); 
		Temperature = Convert_Voltage_To_Temperature(Voltage);
		
		if(Temperature > ALARM_THRESHOLD){
			RAISE_THE_ALARM = 1;
		}else{
			RAISE_THE_ALARM = 0;
		}
	}
}

 /*---------------------------------------------------------------------------
*			Initialize GPIOE, LISD3SH
 *---------------------------------------------------------------------------*/
void initializeTemperatureThread(void){
	kstate.q =	0.15;
	kstate.r =	135;	
	kstate.x =	0.0;
	kstate.p =	1000;
	kstate.k =	0.0;
	
	configADC();
	configureTemperatureTimer();
}

/*----------------------------------------------------------------------------
 *      Initialize the GPIO associated with the Acc interrupt (GPIOE0)
 *---------------------------------------------------------------------------*/

	
/*----------------------------------------------------------------------------
 *      Handler and Callback functions for interrupts
 *---------------------------------------------------------------------------*/

/**
  * @brief  This function handles TIM3 interrupts
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void){
	HAL_TIM_IRQHandler(&TIM_2);
}

/*----------------------------------------------------------------------------
 *      Utility functions for ADC data (filtering and calibration-adjustments)
 *---------------------------------------------------------------------------*/
float Convert_Voltage_To_Temperature(uint32_t Voltage){
	
	float Temperature = Voltage * 1.0f;
	
	Temperature *= 3.30f/0xfff;
	Temperature -= 0.76;
	Temperature /= 0.025;
	Temperature += 25.0;
	
	return Temperature;
}