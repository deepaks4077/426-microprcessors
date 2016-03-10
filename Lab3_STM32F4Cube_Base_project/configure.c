#include "configure.h"

void LIS3DSH_GPIO_CONFIG(void);
GPIO_InitTypeDef GPIOxInit(uint32_t pinNum, uint32_t mode, uint32_t pull, uint32_t speed, uint32_t alternate);

void configure(void){
		
	LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_IntConfigStruct;
	GPIO_InitTypeDef GPIO_E;
	
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;          /* Ppower down or /active mode with output data rate 3.125 / 6.25 / 12.5 / 25 / 50 / 100 / 400 / 800 / 1600 HZ */
  LIS3DSH_InitStruct.Axes_Enable = LIS3DSH_XYZ_ENABLE ;                         /* Axes enable */
  LIS3DSH_InitStruct.Continous_Update = LIS3DSH_ContinousUpdate_Disabled; 			/* Block or update Low/High registers of data until all data is read */
	LIS3DSH_InitStruct.AA_Filter_BW = LIS3DSH_AA_BW_50;												  	/* Choose anti-aliasing filter BW 800 / 400 / 200 / 50 Hz*/
  LIS3DSH_InitStruct.Full_Scale = LIS3DSH_FULLSCALE_2 ;                         /* Full scale 2 / 4 / 6 / 8 / 16 g */
  LIS3DSH_InitStruct.Self_Test = LIS3DSH_SELFTEST_NORMAL ;

	LIS3DSH_IntConfigStruct.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;  
	LIS3DSH_IntConfigStruct.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	LIS3DSH_IntConfigStruct.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;
	
	__HAL_RCC_GPIOE_CLK_ENABLE();
  GPIO_E.Pin = GPIO_PIN_0;
	GPIO_E.Mode = GPIO_MODE_IT_FALLING;
	GPIO_E.Pull = GPIO_PULLDOWN;
	GPIO_E.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE,&GPIO_E);
	
	//NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn,0,1);
	HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);
	
	LIS3DSH_Init(&LIS3DSH_InitStruct);
  LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_IntConfigStruct);
	
}

GPIO_InitTypeDef GPIOxInit(uint32_t pinNum, uint32_t mode, uint32_t pull, uint32_t speed, uint32_t alternate){
	
	GPIO_InitTypeDef init;
	init.Pin = pinNum;
	init.Mode = mode;
	init.Pull = pull;
	init.Speed = speed;
	init.Alternate = alternate;

	return init;
}