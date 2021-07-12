
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart.h"
#include "can.h"


uint8_t* i = (uint8_t*)0x20000404;

void SystemClock_Config(void);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    __enable_irq();
    Uart uart;
    Can can;
    
    if (uart.receivedByte == 0) {
        uart.receivedByte = 2;
    }

    for (uint8_t i = 0; i < 8; i++) {
        can.TxData[i] = (i + 10);
    }

    uint8_t temp=0;
    while (1) {
        HAL_Delay(50);
        can.TxHeader.StdId = 0x0378;
        can.TxData[0] ++;
        // wait for free tx FIFO
        while (HAL_CAN_GetTxMailboxesFreeLevel(&can.hcan) == 0){};
        if (can.send(can.MAIN, 8) != HAL_OK) {
            uart.sendStr("CAN_error");
        }
        HAL_Delay(50);
        while (HAL_CAN_GetTxMailboxesFreeLevel(&can.hcan) == 0){};
        if (can.send(can.UP, 8) != HAL_OK) {
            uart.sendStr("CAN_error");
        }
        HAL_Delay(50);
    }
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
        Error_Handler();
    }
}

void Error_Handler(void) {
    __disable_irq();
    while (1) {
    }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line) {}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
