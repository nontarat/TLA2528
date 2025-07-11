/*
 * tla2528.h
 *
 *  Created on: Mar 28, 2025
 *      Author: user
 */

#ifndef INC_TLA2528_H_
#define INC_TLA2528_H_


#include "stm32h7xx_hal.h"
#include <stdint.h>

void TLA2528_Init(void);
HAL_StatusTypeDef TLA2528_Write_Register(uint8_t reg_address, uint8_t reg_value);
void TLA2528_ReadChannels(void);
uint16_t TLA2528_GetChannel(uint8_t channel);
void TLA2528_StopSequence(void);


#endif /* INC_TLA2528_H_ */
