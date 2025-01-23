#include <stdint.h>
#ifndef _AS7262_H
#define _AS7262_H

#include <Wire.h>
#include <SPI.h>
#include "pinout.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_AS726x.h"
#include "config.h"


typedef struct      
{
  uint8_t temperature;
  float color[AS726x_NUM_CHANNELS];
} AS7262_DataRead_t;

typedef struct
{
  uint8_t temperature;
  float color[AS726x_NUM_CHANNELS];
} AS7262_DataReceived_t;


namespace AS7262
{
  void HardwareInit(void);
  void DataInit(AS7262_DataRead_t *data);
  void ReadData(AS7262_DataRead_t *data);
}

#endif