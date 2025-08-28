#pragma once
#include "freemaster.h"
#include "freemaster_serial.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FMSTR_STM32_DEFAULT_UART
#define FMSTR_STM32_DEFAULT_UART    huart2
#endif

void FMSTR_STM32_SetUart(UART_HandleTypeDef *huart);
UART_HandleTypeDef* FMSTR_STM32_GetUart(void);

extern const FMSTR_SERIAL_DRV_INTF FMSTR_SERIAL_STM32_HAL;

#ifdef __cplusplus
}
#endif
