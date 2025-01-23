#ifndef ADC_H
#define ADC_H

#include <Arduino.h>

extern ADC_HandleTypeDef hadc1;

HAL_StatusTypeDef* ADC_Init(void);
uint32_t ADC_Read(void);
float mapADCValueToPercentage(int adcValue, int minValue, int maxValue);

#endif