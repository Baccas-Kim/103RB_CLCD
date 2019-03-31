/*
 * bsp.h
 *
 *  Created on: 2018. 5. 12.
 *      Author: Baram
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_


#include "def.h"
#include "stm32f1xx_hal.h"


void bspInit(void);



void delay(uint32_t time_ms);
uint32_t millis(void);


#endif /* SRC_BSP_BSP_H_ */
