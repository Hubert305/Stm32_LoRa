#include "ADC.h"
#include "config.h"

#define HAL_MAX_DELAY 1000000 // ms

ADC_HandleTypeDef hadc1;


HAL_StatusTypeDef* ADC_Init()
{
  static HAL_StatusTypeDef status_arr[2];
  __HAL_RCC_ADC1_CLK_ENABLE();
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  status_arr[0] = HAL_ADC_Init(&hadc1);

  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_4CYCLES;
  status_arr[1] = HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  status_arr[2] = HAL_ADC_Start(&hadc1);
  return status_arr;
}

uint32_t ADC_Read()
{
  if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) // Wait until the end of ADC conversion
  {
    return HAL_ADC_GetValue(&hadc1);
  }
  return 0;
}

float mapADCValueToPercentage(int adcValue, int minValue, int maxValue)
{
  if (adcValue > minValue) 
  {
    return 0.00;
  } else if (adcValue < maxValue)
  {
    return 100.00;
  } else 
  {
    float percentage = ((float)(adcValue - minValue) / (maxValue - minValue)) * 100;
    return percentage;
  }
}
