#include "bme280_sensor.h"

Adafruit_BME280 bme;

bool BME_ACTIVE = false;


namespace BME280
{
  void HardwareInit(void)
  {
    Serial.println("[INFO] Trying to connect BME280 sensor...");

    /* I2C INIT */
    Wire.setSDA(SDA_PIN);
    Wire.setSCL(SCL_PIN);

    if (!bme.begin(BME_SENSOR_ADDR))
    {
      Serial.println("[ERR] BME280 sensor not found!");
      BME_ACTIVE = false;
    } else 
    {
      Serial.println("[INFO] BME280 sensor connected.");
      BME_ACTIVE = true;
    }
  }

  void DataInit(BME280_DataRead_t *data)
  {
    data->temperature = 0;
    data->pressure = 0;
  }

  void ReadData(BME280_DataRead_t *data)
  {
    data->temperature = (bme.readTemperature() * 100);     // Magnitude change for easier sending
    data->pressure = (bme.readPressure() / 100.00f);
  }
}