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

extern void configGPIOE(void);
extern void configLISD3SH(void);
extern osSemaphoreId semAcc;

/* Private variables */
//sensitivity_component
float offset[4][3] = {{0.000947840438189, -0.000019936494628 ,-0.000023987290420},
  {-0.000022355958866 ,0.000964651608004, -0.000010465713571},
  {0.000004748462666 ,-0.000004871582500, 0.001011092805443},
	{-0.006723511104368,-0.002245066070008,-0.018506478672045}
};

float Pitch;
float Roll;

/* Private functions */
float* multiplyMatrix(float* input);
void configureAcc(void);
void configureGPIOE(void);
void initSegmentSemaphore(void);
void readAccelerometerData(void);
	
/* CMSIS_OS Thread related functions */
void initializeAccThread(void);
void Thread_Acc(void const *argument);                 // thread function
osThreadId tid_Thread_Acc;                              // thread id
osThreadDef(Thread_Acc, osPriorityNormal, 1, 0);

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_Acc (void) {
  tid_Thread_Acc = osThreadCreate(osThread(Thread_Acc), NULL); // Start Acc_thread
  if (!tid_Thread_Acc) return(-1); 
  return(0);
}

 /*----------------------------------------------------------------------------
*      Thread  'Thread_Acc': Reads and outputs Acc data
 *---------------------------------------------------------------------------*/
void Thread_Acc(void const *argument) {
	while(1){
		osSemaphoreWait(semAcc,osWaitForever);
		readAccelerometerData();
	}
}

 /*---------------------------------------------------------------------------
*			Initialize GPIOE, LISD3SH
 *---------------------------------------------------------------------------*/
void initializeAccThread(void){
	configureGPIOE();
	configureAcc();
}

/*----------------------------------------------------------------------------
 *      Initialize the GPIO associated with the Acc interrupt (GPIOE0)
 *---------------------------------------------------------------------------*/
void configureGPIOE(void){
	configGPIOE();
}

void configureAcc(void){
	configLISD3SH();
}

void readAccelerometerData(void){
		float readings[3];
		float* calibrated_matrix;
		float* kalman_output;
		float Ax, Ay,Az;
		
		LIS3DSH_ReadACC(readings);
	
		kalman_output = multiplyMatrix(readings);
	
		//	readings[0] = kalmanFilter(readings[0],&kSx); 
		//	readings[1] = kalmanFilter(readings[1],&kSy);
		//	readings[2] = kalmanFilter(readings[2],&kSz);
	
		calibrated_matrix = multiplyMatrix(readings);
		Ax = calibrated_matrix[0];
		Ay = calibrated_matrix[1];
		Az = calibrated_matrix[2];
	
		Pitch = atan2(Ax,Az) * 180 / 3.1415926515;
		Roll = atan2(Ay,Az)* 180 / 3.1415926515;
		//printf("Pitch = %f \n", Pitch);
}


/*----------------------------------------------------------------------------
 *      Handler and Callback functions for interrupts
 *---------------------------------------------------------------------------*/

void EXTI0_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/*----------------------------------------------------------------------------
 *      Utility functions for Acc data (filtering and calibration-adjustments)
 *---------------------------------------------------------------------------*/
float* multiplyMatrix(float* input){
	float raw_data[4] = {0};
	float calibrated_matrix[3] = {0};
	int i,j;
	
	raw_data[0] = input[0];
	raw_data[1] = input[1];
	raw_data[2] = input[2];
	raw_data[3] = 1;
	
	for (i=0; i<3; i++){
		for (j=0; j<4; j++){
			calibrated_matrix[i] += raw_data[j]*offset[j][i];
		}
	}
	
	return calibrated_matrix;
}