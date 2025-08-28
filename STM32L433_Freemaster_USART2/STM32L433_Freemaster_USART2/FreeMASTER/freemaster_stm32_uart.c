#include "freemaster.h"
#include "freemaster_serial.h"
#include "freemaster_stm32_uart.h"

extern UART_HandleTypeDef FMSTR_STM32_DEFAULT_UART;
static UART_HandleTypeDef *s_huart = &FMSTR_STM32_DEFAULT_UART;

#ifndef FMSTR_TRUE
#define FMSTR_TRUE  (1U)
#define FMSTR_FALSE (0U)
#endif

#ifndef UART_FLAG_RXNE_RXFNE
#define UART_FLAG_RXNE_RXFNE UART_FLAG_RXNE
#endif
#ifndef UART_FLAG_TXE_TXFNF
#define UART_FLAG_TXE_TXFNF  UART_FLAG_TXE
#endif
#ifndef UART_IT_RXFNE
#define UART_IT_RXFNE        UART_IT_RXNE
#endif
#ifndef UART_IT_TXFNF
#define UART_IT_TXFNF        UART_IT_TXE
#endif

static inline void fmstr_uart_clear_errors(UART_HandleTypeDef *huart)
{
#ifdef __HAL_UART_CLEAR_PEFLAG
    __HAL_UART_CLEAR_PEFLAG(huart);
#endif
#ifdef __HAL_UART_CLEAR_FEFLAG
    __HAL_UART_CLEAR_FEFLAG(huart);
#endif
#ifdef __HAL_UART_CLEAR_NEFLAG
    __HAL_UART_CLEAR_NEFLAG(huart);
#endif
#ifdef __HAL_UART_CLEAR_OREFLAG
    __HAL_UART_CLEAR_OREFLAG(huart);
#endif
#ifdef __HAL_UART_CLEAR_IDLEFLAG
    __HAL_UART_CLEAR_IDLEFLAG(huart);
#endif
}

static FMSTR_BOOL _stm32_init(void)
{
    if (!s_huart || !s_huart->Instance) return FMSTR_FALSE;
    fmstr_uart_clear_errors(s_huart);
    return FMSTR_TRUE;
}

static void _stm32_enable_tx(FMSTR_BOOL enable)
{
    if (enable)
        __HAL_UART_ENABLE_IT(s_huart, UART_IT_TXFNF);
    else
        __HAL_UART_DISABLE_IT(s_huart, UART_IT_TXFNF);
}

static void _stm32_enable_rx(FMSTR_BOOL enable)
{
    if (enable)
    {
        __HAL_UART_ENABLE_IT(s_huart, UART_IT_RXFNE);
#ifdef UART_IT_ERR
        __HAL_UART_ENABLE_IT(s_huart, UART_IT_ERR);
#endif
    }
    else
    {
        __HAL_UART_DISABLE_IT(s_huart, UART_IT_RXFNE);
#ifdef UART_IT_ERR
        __HAL_UART_DISABLE_IT(s_huart, UART_IT_ERR);
#endif
    }
}

static FMSTR_BOOL _stm32_rx_ready(void)
{
    return (__HAL_UART_GET_FLAG(s_huart, UART_FLAG_RXNE_RXFNE) != 0U) ? FMSTR_TRUE : FMSTR_FALSE;
}

static FMSTR_BOOL _stm32_tx_ready(void)
{
    return (__HAL_UART_GET_FLAG(s_huart, UART_FLAG_TXE_TXFNF) != 0U) ? FMSTR_TRUE : FMSTR_FALSE;
}

static FMSTR_BOOL _stm32_get_char(FMSTR_U8 *pch)
{
    if (!_stm32_rx_ready()) return FMSTR_FALSE;
#if defined(USART_RDR_RDR)
    *pch = (FMSTR_U8)(s_huart->Instance->RDR & 0xFFU);
#else
    *pch = (FMSTR_U8)(s_huart->Instance->DR & 0xFFU);
#endif
    return FMSTR_TRUE;
}

static FMSTR_BOOL _stm32_put_char(FMSTR_U8 ch)
{
    if (!_stm32_tx_ready()) return FMSTR_FALSE;
#if defined(USART_TDR_TDR)
    s_huart->Instance->TDR = ch;
#else
    s_huart->Instance->DR  = ch;
#endif
    return FMSTR_TRUE;
}

static void _stm32_flush(void) { /* optional: wait for TC */ }

static void _stm32_isr(void)
{
    if (__HAL_UART_GET_FLAG(s_huart, UART_FLAG_ORE)) __HAL_UART_CLEAR_OREFLAG(s_huart);
    if (__HAL_UART_GET_FLAG(s_huart, UART_FLAG_FE))  __HAL_UART_CLEAR_FEFLAG(s_huart);
    if (__HAL_UART_GET_FLAG(s_huart, UART_FLAG_NE))  __HAL_UART_CLEAR_NEFLAG(s_huart);
#ifdef UART_FLAG_PE
    if (__HAL_UART_GET_FLAG(s_huart, UART_FLAG_PE))  __HAL_UART_CLEAR_PEFLAG(s_huart);
#endif
}

const FMSTR_SERIAL_DRV_INTF FMSTR_SERIAL_STM32_HAL = {
    .Init           = _stm32_init,
    .EnableTransmit = _stm32_enable_tx,
    .EnableReceive  = _stm32_enable_rx,
    .GetChar        = _stm32_get_char,
    .PutChar        = _stm32_put_char,
    .Flush          = _stm32_flush,
    .Isr            = _stm32_isr
};

void FMSTR_STM32_SetUart(UART_HandleTypeDef *huart) { s_huart = huart; }
UART_HandleTypeDef* FMSTR_STM32_GetUart(void) { return s_huart; }
