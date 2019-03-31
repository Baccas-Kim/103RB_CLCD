/*
 * flash.c
 *
 *  Created on: 2018. 7. 21.
 *      Author: Baram
 */



#include "flash.h"




#define FLASH_MAX_PAGE_COUNT    128





typedef struct
{
  uint32_t address;
  uint32_t length;
} flash_tbl_t;



flash_tbl_t flash_tbl[FLASH_MAX_PAGE_COUNT];



static int32_t getPage(uint32_t Address);



void flashInit(void)
{
  uint32_t i;


  for (i=0; i<FLASH_MAX_PAGE_COUNT; i++)
  {
    flash_tbl[i].address = 0x08000000 + ( i * FLASH_PAGE_SIZE );
    flash_tbl[i].length  = FLASH_PAGE_SIZE;
  }
}

bool flashErase(uint32_t addr, uint32_t length)
{
  bool ret = true;
  int32_t first_page = 0;
  int32_t num_page = 0;
  uint32_t SECTORError = 0;
  FLASH_EraseInitTypeDef EraseInitStruct;



  HAL_FLASH_Unlock();



  first_page = getPage(addr);
  num_page   = getPage(addr + length - 1) - first_page + 1;


  if (first_page < 0)
  {
    return false;
  }

  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = flash_tbl[first_page].address;
  EraseInitStruct.NbPages     = num_page;

  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
  {
    ret = false;
  }


  HAL_FLASH_Lock();


  return ret;
}

bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint64_t data;


  // 2byte align
  if (addr%2 != 0)
  {
    return false;
  }

  HAL_FLASH_Unlock();

  for (uint32_t i=0; i<length; i += 2)
  {
    data = (p_data[i+1] << 8) | (p_data[i+0] << 0);

    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr + i, data) != HAL_OK)
    {
      ret = false;
      break;
    }
  }


  HAL_FLASH_Lock();

  return ret;
}

static int32_t getPage(uint32_t address)
{
  uint32_t i;
  int32_t page = -1;


  for (i=0; i<FLASH_MAX_PAGE_COUNT; i++)
  {
    if (address >= flash_tbl[i].address && address < (flash_tbl[i].address + flash_tbl[i].length))
    {
      page = i;
      break;
    }
  }

  return page;
}
