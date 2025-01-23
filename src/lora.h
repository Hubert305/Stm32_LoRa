#ifndef _LORA_H
#define _LORA_H

#include <Arduino.h>
#include <Wire.h>
#include "LoRaRadio.h"
#include "bme280_sensor.h"
#include "AS7262_sensor.h"

typedef struct      
{
  BME280_DataRead_t BME_sensorData;
  AS7262_DataRead_t AS_sensorData;
} Sensors_DataRead;


namespace LoRa
{
  void ShieldInit(void);

  void SendRequest(void);
  void SendResponse(Sensors_DataRead *data);
  void ReadData(uint8_t message[]);
}

#endif
