#ifndef _CONFIG_H
#define _CONFIG_H

// MAIN SOFTWARE CONFIG
#define MODE_PIN PA0

// COMMUNICATION CONFIG
#define SERIAL_SPEED 115200u
#define SEND_PERIOD_MS 5000u
#define MESSAGE_SIZE 22u
#define EOT 0x04 // End-of-Transmission character

// BME280 SENSOR CONFIG
#define BME_SENSOR_ADDR 0x77
#define SEA_LEVEL_PRESSURE 1013.25

// SEN0193 SENSOR CONFIG
#define SEN0193_MIN_VALUE 2900 // Record the sensor value when the probe is exposed to the air
#define SEN0193_MAX_VALUE 1740 // Record the sensor value when the probe is exposed to the water as

// AS7262 SENSOR CONFIG
#define AS7262_SENSOR_ADDR 0x49

#endif
