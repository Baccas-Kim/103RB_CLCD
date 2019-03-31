/*
 * ap.c
 *
 *  Created on: 2018. 5. 12.
 *      Author: Baram
 */


#include "ap.h"



bool runWriteEeprom(void);
bool runReadEeprom(void);
bool runEraseEeprom(void);



void apInit(void)
{
  uartOpen(_DEF_UART1, 115200);
  lcd_Init(16,2);
}

void apMain(void)
{
  lcd_string("Hello World!");
  HAL_Delay(2000);
  while(1)
  {
    lcd_setCurStr(0,1,"Hello OROCA");
    ledToggle(0);
    delay(500);
    lcd_string("Hello World!");
    //lcd_clear();
    delay(500);
  }
}


bool runEraseEeprom(void)
{
  bool ret = true;
  uint8_t data[8] = {0, };


  uartPrintf(_DEF_UART1, "EraseEeprom..");
  if (eepromWrite(0, data, 8) == true)
  {
    uartPrintf(_DEF_UART1, "OK\n");
  }
  else
  {
    uartPrintf(_DEF_UART1, "Fail\n");
    ret = false;
  }

  return ret;
}

bool runReadEeprom(void)
{
  bool ret = true;
  uint8_t data;

  uartPrintf(_DEF_UART1, "ReadEeprom..\n");

  for (int i=0; i<8; i++)
  {
    eepromRead(i, &data, 1);
    uartPrintf(_DEF_UART1, "0x%02X : 0x%02X \n", i, data);
  }

  return ret;
}

bool runWriteEeprom(void)
{
  bool ret = true;
  uint8_t data;

  uartPrintf(_DEF_UART1, "WriteEeprom..");

  for (uint32_t i=0; i<8; i++)
  {
    data = i;
    ret = eepromWrite(i, &data, 1);

    if (ret == false)
    {
      break;
    }
  }

  if (ret == true)
  {
    uartPrintf(_DEF_UART1, "OK\n");
  }
  else
  {
    uartPrintf(_DEF_UART1, "Fail\n");
  }

  return ret;
}
