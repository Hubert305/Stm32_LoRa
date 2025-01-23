#include "AS7262_sensor.h"

Adafruit_AS726x ams;

bool AS_ACTIVE = false;


namespace AS7262{
  void HardwareInit(void)
  {
    Serial.println("[INFO] Trying to connect AS7262 sensor...");

    if (!ams.begin())
    {
      Serial.println("[ERR] AS7262 sensor not found!");
      AS_ACTIVE = false;
    } else 
    {
      Serial.println("[INFO] AS7262 sensor connected.");
      AS_ACTIVE = true;
    }
  }
  
  void DataInit(AS7262_DataRead_t *data)
  {
    data->temperature = 0;
    memset(data->color, 0, sizeof(float)*AS726x_NUM_CHANNELS);
  }

  void ReadData(AS7262_DataRead_t *data)
  {
    ams.drvOn();
    ams.startMeasurement(); //begin a measurement
    bool ready = false;

    while(!ready)
    {
      delay(5);
      ready = ams.dataReady();
    }
    ams.drvOff();
    ams.readCalibratedValues(data->color);
    data->temperature = ams.readTemperature();
  }
}