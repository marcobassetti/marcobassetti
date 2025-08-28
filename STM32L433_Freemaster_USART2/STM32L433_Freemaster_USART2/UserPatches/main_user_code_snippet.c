/* === USER CODE additions for Core/Src/main.c === */
#include "freemaster.h"
#include "freemaster_cfg.h"
#include "freemaster_stm32_uart.h"

/* Example variable exported via TSA */
volatile float myVar = 0.0f;

/* TSA (optional) */
FMSTR_TSA_TABLE_BEGIN(main_tsa)
    FMSTR_TSA_RW_VAR(myVar, FMSTR_TSA_FLOAT)
FMSTR_TSA_TABLE_END()

FMSTR_TSA_TABLE_LIST_BEGIN()
    FMSTR_TSA_TABLE(main_tsa)
FMSTR_TSA_TABLE_LIST_END()

/* In main() after MX_USART2_UART_Init(): */
FMSTR_STM32_SetUart(&huart2);
FMSTR_Init();

/* In while(1): */
myVar += 0.01f;
FMSTR_Poll();   /* optional but recommended */
/*
 *
 */
