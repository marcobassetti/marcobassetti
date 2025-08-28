/* freemaster_cfg.h — STM32L433 / USART2 / SHORT-INT */

#ifndef FREEMASTER_CFG_H
#define FREEMASTER_CFG_H

/* Cortex-M platform */
#define FMSTR_PLATFORM_CORTEX_M         1

/* Enable driver */
#define FMSTR_DISABLE                   0

/* Driver mode */
#define FMSTR_LONG_INTR                 0
#define FMSTR_SHORT_INTR                1   /* short ISR */
#define FMSTR_POLL_DRIVEN               0

/* Transport: Serial */
#define FMSTR_TRANSPORT                 FMSTR_SERIAL

/* Low-level serial driver interface we provide */
extern const FMSTR_SERIAL_DRV_INTF FMSTR_SERIAL_STM32_HAL;
#define FMSTR_SERIAL_DRV                (&FMSTR_SERIAL_STM32_HAL)

/* Protocol buffer (0 = auto) */
#define FMSTR_COMM_BUFFER_SIZE          0

/* RX queue used in SHORT-INT mode */
#define FMSTR_COMM_RQUEUE_SIZE          64

/* AppCmd */
#define FMSTR_USE_APPCMD                1
#define FMSTR_APPCMD_BUFF_SIZE          32

/* Scope */
#define FMSTR_USE_SCOPE                 1
#define FMSTR_MAX_SCOPE_VARS            8

/* Recorder */
#define FMSTR_USE_RECORDER              1
#define FMSTR_REC_BUFF_SIZE             512
#define FMSTR_REC_TIMEBASE              FMSTR_REC_BASE_MILLISEC(1)

/* TSA */
#define FMSTR_USE_TSA                   1
#define FMSTR_USE_TSA_INROM             1
#define FMSTR_USE_TSA_SAFETY            0   /* set 1 if you want write protection */

#endif /* FREEMASTER_CFG_H */
