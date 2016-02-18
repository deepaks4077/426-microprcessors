#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "config.h"

ADC_HandleTypeDef ADC1_Handle;

/** Configure and start ADC1 -------------------------------------------------------------*/
void ADC_config(void){

	ADC_InitTypeDef* ADC_Init;
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