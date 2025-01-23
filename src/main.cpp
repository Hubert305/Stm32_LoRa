#include "main.h"
#include "lora.h"
#include "bme280_sensor.h"
#include "AS7262_sensor.h"
#include "ADC.h"

uint8_t requestMessage[1];
uint8_t receivedMessage[MESSAGE_SIZE];
bool SlaveRequestProcessingFLAG = false;

bool interruptState = false;
volatile int interruptLedCount = 0;
char receivedChar = 0;
BME280_DataRead_t BME_read;
BME280_DataReceived_t BME_received;
AS7262_DataRead_t AS_read;
AS7262_DataReceived_t AS_received;
Sensors_DataRead sensorsData;

// Create instance of TIM2
HardwareTimer *MyTim = new HardwareTimer(TIM2);


void setup()
{
  pinMode(MODE_PIN, INPUT_PULLUP);
  HAL_StatusTypeDef* init_errors = initRegs();

  if (isSlave())
  {
    Serial.println("[INFO] ADC Init Errors: ");
    Serial.print("  ADC Initialization: ");
    Serial.println(init_errors[0]);
    Serial.print("  ADC Channel Configuration: ");
    Serial.println(init_errors[1]);
    Serial.print("  ADC Start: ");
    Serial.println(init_errors[2]);
    Serial.println("SLAVE mode is ON");
    
    /* Sensors HardwareInit */
    BME280::HardwareInit();
    AS7262::HardwareInit();
  } else
  {
    Serial.println("MASTER mode is ON");
  }
}


void loop()
{
  if (isSlave())
  { /* Process request */
    if ((loraRadio.read(requestMessage) == EOT) && (SlaveRequestProcessingFLAG == false))
    {
      SlaveRequestProcessingFLAG = true;
      MyTim->resume(); // Start TIM2 and blinking
      memset(requestMessage, 0, 1);
      Serial.println("[INFO] Data request received");
    }

    if (SlaveRequestProcessingFLAG)
    { /* Read data from sensor and send it */
      if (BME_ACTIVE)
      {
        BME280::ReadData(&BME_read);
        sensorsData.BME_sensorData = BME_read;
      }
      if (AS_ACTIVE)
      {
        AS7262::ReadData(&AS_read);
        sensorsData.AS_sensorData = AS_read;
      }
      LoRa::SendResponse(&sensorsData);
      delay(2000);  // Wait for the message to be sent
      Serial.println("Data sent");
      SlaveRequestProcessingFLAG = false;
    }
  } else
  {

    if (/*interruptState || */receivedChar == 'x')
    { /* Send request to slave */
      MyTim->resume();     // Start TIM2 and blinking
      LoRa::SendRequest();
      //interruptState = !interruptState;
      //clear serial buffer
      //while (Serial.available() > 0) Serial.read();
      receivedChar = 0;
      //delay(1000);
    }
    if(Serial.available()>0)
    {
        receivedChar = Serial.read();
    }

    if (loraRadio.read(receivedMessage) > 0)
    {
      LoRa::ReadData(receivedMessage);
    }
  } 
}

HAL_StatusTypeDef* initRegs(void)
{
  LoRa::ShieldInit(); // There is no need to pass module type, it is defined in config.h

  /* Init data container structs */
  BME280::DataInit(&BME_read);
  AS7262::DataInit(&AS_read);

  /* Configure PA5 (builtin LED) */
  pinMode(LED_GREEN, OUTPUT); 

  /* Init Timer */
  MyTim->setOverflow(30, HERTZ_FORMAT); // Set interrupt interval ~33ms
  MyTim->attachInterrupt(updateLedState);

  HAL_StatusTypeDef* status_arr = 0;  // Array of ADC error statuses
  if (isSlave())
  {
    status_arr = ADC_Init(); 
  }
  else
  {
    attachInterrupt(digitalPinToInterrupt(BOARD_BTN), ButtonClickInterrupt, RISING);
  }
  return status_arr;
}

void updateLedState(void)
{
  digitalWrite(LED_GREEN, !digitalRead(LED_GREEN)); // Change state of LED
  interruptLedCount++;
  if (interruptLedCount >= 8) // After 8th change -> turn off timer
  { 
    MyTim->pause();
    interruptLedCount = 0;
    digitalWrite(LED_GREEN, false); // In case of LED staying turned ON
  }
}

void ButtonClickInterrupt(void)
{
  interruptState = !interruptState;
}

bool isSlave(void) // Indicate device mode 0 = Master, 1 = Slave
{
  return digitalRead(MODE_PIN) == HIGH;
}
