/*
 * hw.c
 *
 *  Created on: 2018. 5. 12.
 *      Author: Baram
 */


#include "hw.h"





void hwInit(void)
{
  bspInit();
  swtimerInit();
  ledInit();
  buttonInit();
  uartInit();
  timerInit();
  flashInit();
  eepromInit();
}

