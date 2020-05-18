/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "print_debug.h"
#include "I2cLcd.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void delay_us(uint16_t us);
float Start_GetRange(void);
float IOE_SR05_GetRange(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BT1_Pin GPIO_PIN_0
#define BT1_GPIO_Port GPIOA
#define IOE_SR05_EN_Pin GPIO_PIN_4
#define IOE_SR05_EN_GPIO_Port GPIOF
#define LD4_Pin GPIO_PIN_8
#define LD4_GPIO_Port GPIOC
#define LD3_Pin GPIO_PIN_9
#define LD3_GPIO_Port GPIOC
#define ECHO_Pin GPIO_PIN_11
#define ECHO_GPIO_Port GPIOC
#define TRIG_Pin GPIO_PIN_12
#define TRIG_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */
#define USE_IOE_SR05	0

#define TRIG_H()	TRIG_GPIO_Port->BSRR=TRIG_Pin
#define TRIG_L()	TRIG_GPIO_Port->BRR=TRIG_Pin

#define IOE_SR05_H()	IOE_SR05_EN_GPIO_Port->BSRR=IOE_SR05_EN_Pin
#define IOE_SR05_L()	IOE_SR05_EN_GPIO_Port->BRR=IOE_SR05_EN_Pin
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
