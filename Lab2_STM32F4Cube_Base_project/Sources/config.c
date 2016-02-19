#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "config.h"

#define DECIMAL 									 ((uint16_t)0x0400)
#define SEG_A											 ((uint16_t)0x0080)
#define SEG_B	 		      	         ((uint16_t)0x0100)  
#define SEG_C     			           ((uint16_t)0x0200)  
#define SEG_D                			 ((uint16_t)0x0400)  
#define SEG_E             			   ((uint16_t)0x0800)  
#define SEG_F       			         ((uint16_t)0x1000)  
#define SEG_G 			               ((uint16_t)0x2000)
#define CTRL_1                		 ((uint16_t)0x0100)  
#define CTRL_2             			   ((uint16_t)0x0200)  
#define CTRL_3       			         ((uint16_t)0x0400)  
#define CTRL_4 			               ((uint16_t)0x0800)


ADC_HandleTypeDef ADC1_Handle;
GPIO_InitTypeDef GPIO_D;
GPIO_InitTypeDef GPIO_E;

/** Configure and start ADC1 -------------------------------------------------------------*/
void ADC_config(void){

	ADC_ChannelConfTypeDef ADC1_Channel;
	
	ADC1_Handle.Instance = ADC1;
	ADC1_Handle.NbrOfCurrentConversionRank = 1;
	ADC1_Handle.State = HAL_ADC_STATE_READY;
	
	/* Configure init handler ----------------------------------------------------*/
	ADC1_Handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	ADC1_Handle.Init.Resolution = ADC_RESOLUTION_12B;
	ADC1_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC1_Handle.Init.ScanConvMode = DISABLE;
	ADC1_Handle.Init.EOCSelection = DISABLE;
	ADC1_Handle.Init.ContinuousConvMode = ENABLE;
	ADC1_Handle.Init.DMAContinuousRequests = DISABLE;
	ADC1_Handle.Init.NbrOfConversion = 1;
	ADC1_Handle.Init.DiscontinuousConvMode = DISABLE;
	ADC1_Handle.Init.NbrOfDiscConversion = 0;
	ADC1_Handle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	ADC1_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	
	/* Configure the data acquisition channel ------------------------------------*/
	ADC1_Channel.Channel = ADC_CHANNEL_16;
	ADC1_Channel.Rank = 1;
	ADC1_Channel.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	ADC1_Channel.Offset = 0;

	/* Enable the ADC clock ------------------------------------------------------*/
	__ADC1_CLK_ENABLE();
	
	/* Initialize ADC1 -----------------------------------------------------------*/
	if(HAL_ADC_Init(&ADC1_Handle) != HAL_OK){
		Error_Handler(ADC_INIT_FAIL);
	}

	/* Configure the channel -> MUST BE DONE AFTER STARTING THE CLOCK ------------*/
	HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC1_Channel);
	
	/* Start the ADC -------------------------------------------------------------*/
	HAL_ADC_Start(&ADC1_Handle);
}

void GPIO_config(void){
	
	GPIO_D.Pin = CTRL_1 | CTRL_2 | CTRL_3 | CTRL_4 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_D.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_D.Pull = GPIO_PULLUP;
	GPIO_D.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_E.Pin = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G;
	GPIO_E.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_E.Pull = GPIO_PULLUP;
	GPIO_E.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	HAL_GPIO_Init(GPIOD,&GPIO_D);
	HAL_GPIO_Init(GPIOE,&GPIO_E);
}


