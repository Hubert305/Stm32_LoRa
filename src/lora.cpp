#include <stdint.h>
#include "lora.h"
#include "config.h"
#include "bme280_sensor.h"
#include "ADC.h"
#include "main.h"

HardwareSerial SerialLora(PA10, PA9);

namespace LoRa{
  void ShieldInit(void)
  {
    Serial.begin(SERIAL_SPEED);
    if (isSlave())
    {
      Serial.println("LoRa SLAVE Module");
    }else
    {
      Serial.println("LoRa MASTER Module");
    }
      
    while (!loraRadio.begin(&SerialLora))
    {
      Serial.println("[INFO] LoRa Shield not ready!");
      delay(1000); /* Give module 1s to init */
    }
    Serial.println("[INFO] Shield ready!");
  }



  void SendRequest(void)
  {
    uint8_t message[1];
    message[0] = 0xFF;

    Serial.println("[INFO] Sending new request");

    loraRadio.write(message, 1);
  }

  void SendResponse(Sensors_DataRead *sensor)
  {
    uint8_t message[MESSAGE_SIZE];

    // Split each 16-bit data to 2x8-bit ones with bit masking
    message[0] = (sensor->BME_sensorData.temperature & 0xFF00) >> 8;
    message[1] = (sensor->BME_sensorData.temperature & 0x00FF);
    message[2] = (sensor->BME_sensorData.pressure & 0xFF00) >> 8;
    message[3] = (sensor->BME_sensorData.pressure & 0x00FF);

    float soil_humidity =  mapADCValueToPercentage(ADC_Read(), SEN0193_MIN_VALUE, SEN0193_MAX_VALUE);
    message[4] = (int(soil_humidity*100) & 0xFF00) >> 8;
    message[5] = (int(soil_humidity*100) & 0x00FF);

    message[6] = (sensor->AS_sensorData.temperature & 0xFF00) >> 8;
    message[7] = (sensor->AS_sensorData.temperature & 0x00FF);

    for (uint8_t i = 0; i < AS726x_NUM_CHANNELS; i++) // Inserting read color data for all 6 channels into the message
    {
      uint8_t ind = 8+2*i;
      message[ind] = (int(sensor->AS_sensorData.color[i]) & 0xFF00) >> 8;
      message[ind+1] = (int(sensor->AS_sensorData.color[i]) & 0x00FF);
    }
    message[20] = EOT; //Attach EOT at the end of string

    Serial.println("[INFO] Sending response");

    loraRadio.write(message, MESSAGE_SIZE);
    delay(3000);
  }

  void ReadData(uint8_t message[])
  {
    loraRadio.read(message);

    // temperature and pressure
    float received_temperature = (float)((message[0] << 8) + message[1]) / 100;
    float received_pressure = (float)((message[2] << 8) + message[3]);
    float received_humidity = (float)((message[4] << 8) + message[5]) / 100;
    float received_temperature_on_sensor = (message[6] << 8) + message[7];
    float received_color_v = (message[8] << 8) + message[9];
    float received_color_b = (message[10] << 8) + message[11];
    float received_color_g = (message[12] << 8) + message[13];
    float received_color_y = (message[14] << 8) + message[15];
    float received_color_o = (message[16] << 8) + message[17];
    float received_color_r = (message[18] << 8) + message[19];

    String sensors_response[] = 
    {
      "[INFO] Response received:",
      "Temperature: " + String(received_temperature),// + " \u00b0C",
      "Pressure: " + String(received_pressure),// + " hPa",
      "Soil humidity: " + String(received_humidity),// + "%",
      "Temperature on sensor: " + String(received_temperature_on_sensor),// + " \u00b0C",
      "Violet: " + String(received_color_v),
      "Blue: " + String(received_color_b),
      "Green: " + String(received_color_g),
      "Yellow: " + String(received_color_y),
      "Orange: " + String(received_color_o),
      "Red: " + String(received_color_r)
    };

    uint8_t responseLength = (sizeof(sensors_response) / sizeof(sensors_response[0]));
    for (uint8_t idx = 0; idx < responseLength; idx++)
    {
      Serial.println(sensors_response[idx]);
    }
  }
}
