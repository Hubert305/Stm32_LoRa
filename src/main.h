#ifndef _MAIN_H
#define _MAIN_H

#include "pinout.h"
#include "config.h"


extern bool BME_ACTIVE;
extern bool AS_ACTIVE;

void ButtonClickInterrupt(void);
HAL_StatusTypeDef* initRegs(void);
void updateLedState(void);
bool isSlave(void);

#endif /* _MAIN_H */
