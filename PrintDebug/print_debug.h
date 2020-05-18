/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PRINT_DEBUG_H
#define __PRINT_DEBUG_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include "main.h"


#ifdef PRINT_DEBUG_RTOS
extern osSemaphoreId print_debug;
#endif


/* USER CODE BEGIN 0 */
/* Global variables ---------------------------------------------------------*/
#if (defined(__GNUC__) && !defined(__CC_ARM))
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */
/* USER CODE END 0 */

/* Global variables ---------------------------------------------------------*/


/* USER CODE BEGIN 1 */
/* USER CODE END 1 */
void PrintDebug(const char *format, ...);


/* USER CODE BEGIN 2 */
/* USER CODE END 2 */

#ifdef __cplusplus
}
#endif
#endif /*__PRINT_DEBUG_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
