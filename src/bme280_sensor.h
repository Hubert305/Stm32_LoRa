#ifndef _BME280_H
#define _BME280_H

#include <Wire.h>
#include <SPI.h>
#include "pinout.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#include "config.h"


typedef struct      
{
  uint16_t temperature;
  uint16_t pressure;
} BME280_DataRead_t;

typedef struct
{
  float temperature;
  float pressure;
} BME280_DataReceived_t;


namespace BME280
{
  void HardwareInit(void);
  void DataInit(BME280_DataRead_t *data);
  void ReadData(BME280_DataRead_t *data);
}

#endif