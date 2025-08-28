# STM32L433 FreeMASTER (USART2, Short-Interrupt) — Starter Kit for STM32CubeIDE

This package includes:
- A preconfigured **CubeMX .ioc** for STM32L433 (USART2 @ 115200, HSI16 as SYSCLK, SWD).
- A minimal **FreeMASTER low-level UART driver** for STM32/HAL (interrupt-driven, SHORT-INT).
- A project-side **freemaster_cfg.h**.
- Snippets to paste into `stm32l4xx_it.c` and `main.c` in the user sections.

> **Important:** The official FreeMASTER Communication Driver source files are not included here due to licensing.
> You must download them from NXP and add them to your project (see steps below).

## 1) Generate the CubeIDE project
1. Open `STM32L433_Freemaster_USART2.ioc` with **STM32CubeMX** (any recent 6.x is ok).
2. In *Project Manager → Project* set a directory and choose **Toolchain/IDE: STM32CubeIDE**.
3. Generate code (CubeMX will create a full CubeIDE project with HAL/CMSIS).

## 2) Add FreeMASTER core sources (once per workspace)
Download the **FreeMASTER Communication Driver v3** from NXP. In the generated project, add the following folders/files (typical layout):
- `Middlewares/FreeMASTER/src/common/*.c`
- `Middlewares/FreeMASTER/src/platforms/cortex-m/*.c`
- `Middlewares/FreeMASTER/include/*.h`

Then, add `Middlewares/FreeMASTER/include` to your include path in **Project → Properties → C/C++ General → Paths and Symbols**.

> Search in the NXP package for folders named `freemaster/src` and `freemaster/include`.
> Alternatively, keep them under `Middlewares/Third_Party/FreeMASTER/…` as you prefer.

## 3) Add the provided integration files
Copy the contents of `FreeMASTER/` from this kit into your project (e.g. `Core/Inc` for headers and `Core/Src` for `.c`), or place them in a dedicated folder and add it to include paths:
- `freemaster_cfg.h`
- `freemaster_stm32_uart.h`
- `freemaster_stm32_uart.c`

## 4) Edit ISR (very important)
Open `Core/Src/stm32l4xx_it.c`. In `USART2_IRQHandler`, **do not** call `HAL_UART_IRQHandler(&huart2)`. Instead, call the FreeMASTER ISR entry point:
```c
#include "freemaster.h"
#include "freemaster_cfg.h"

void USART2_IRQHandler(void)
{
    FMSTR_SerialIsr();
}
```
If CubeMX re-generates the file, make sure the above remains inside the *USER CODE* section.

## 5) Edit main.c (minimal glue)
In `main.c`:
- Add includes and TSA (optional).
- After `MX_USART2_UART_Init()` call `FMSTR_STM32_SetUart(&huart2); FMSTR_Init();`
- Call `FMSTR_Poll()` in the main loop (optional but recommended).

Use the snippet in `UserPatches/main_user_code_snippet.c` (copy only the USER CODE parts).

## 6) CubeIDE build settings
- Add include paths for your FreeMASTER folders and for the directory where you place `freemaster_cfg.h` & `freemaster_stm32_uart.h`.
- Ensure optimization doesn't remove variables intended for TSA (mark them `volatile`).

## 7) FreeMASTER PC Tool
- Interface: **Serial**
- COM port: your USB-UART bridge
- Baudrate: 115200
- Protocol: FreeMASTER v3 Serial
- Load the ELF file in FreeMASTER to browse symbols; enable TSA for write protection if needed.

## Files in this kit
- `STM32L433_Freemaster_USART2.ioc` — CubeMX project file
- `FreeMASTER/freemaster_cfg.h`
- `FreeMASTER/freemaster_stm32_uart.h`
- `FreeMASTER/freemaster_stm32_uart.c`
- `UserPatches/stm32l4xx_it.c.insert.txt` — ISR snippet
- `UserPatches/main_user_code_snippet.c` — User additions for main.c

---

Tested conceptually against STM32L433 (USART2 pins default to **PA2** TX, **PA3** RX; HSI16 as SYSCLK).
If your board differs, adjust in the .ioc before generating code.
