/* Includes ------------------------------------------------------------------*/
#include "print_debug.h"
#include <stdarg.h>

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */

/* Global variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
// Semaphore for print debug
#ifdef PRINT_DEBUG_RTOS
osSemaphoreId print_debug = NULL;
osSemaphoreDef(print_debug);
#endif

/* USER CODE BEGIN 2 */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART3 and Loop until the end of transmission */  
    while (HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1,1000)!= HAL_OK)
    {
        ;
    }
    
    return ch;
}

/**
  * @brief  Retargets the C library scanf function to the USART.
  * @param  None
  * @retval None
  */
GETCHAR_PROTOTYPE
{
    /* Place your implementation of fgetc here */
    /* e.g. read a character on USART3 and loop until the end of read */
    uint8_t ch = 0;
    
    while (HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1,1000) != HAL_OK)
    {
        ;
    }
    return ch;
}
/* USER CODE END 2 */




void PrintDebug(const char *format, ...)
{
#ifdef PRINT_DEBUG_RTOS
    if ((print_debug == 0) || (print_debug == NULL))
    {
        print_debug = osSemaphoreCreate(osSemaphore(print_debug), 1);
    }
  
    osSemaphoreWait(print_debug, osWaitForever);
    va_list vargs;
    va_start(vargs, format);
    vprintf(format, vargs);
    va_end(vargs);
    osDelay(1);
    osSemaphoreRelease(print_debug);
#else
    va_list vargs;
    va_start(vargs, format);
    vprintf(format, vargs);
    va_end(vargs);
#endif
}

/**
  * @}
  */
 
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
