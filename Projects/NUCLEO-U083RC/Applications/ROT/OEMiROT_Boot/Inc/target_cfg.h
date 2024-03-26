/*
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __STM32U0xx_CFG_H__
#define __STM32U0xx_CFG_H__

#include "uart_stdout.h"
#ifdef FIH_PROFILE_ON
#include "fih.h"
#endif
#define DRIVER_STDIO    Driver_USART0

#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

typedef enum
{
  OEMIROT_FALSE = 0U,
  OEMIROT_TRUE = !OEMIROT_FALSE
} OEMIROT_BoolTypeDef;

typedef enum
{
  OEMIROT_DISABLE = 0U,
  OEMIROT_ENABLE = !OEMIROT_DISABLE
} OEMIROT_FunctionalState;

#endif /* __STM32U0xx_CFG_H__ */
