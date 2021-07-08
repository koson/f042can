#include "uart.h"

Uart *Uart::pThis = nullptr;

Uart::Uart() {
    pThis = this;
    init();
}

void Uart::sendByte(uint8_t byte) {
    while (!(USART2->ISR & USART_ISR_TXE))
        ;
    USART2->TDR = byte;
}

void Uart::sendStr(const char *str) {
    while (*str) {
        sendByte(*((uint8_t *)str++));
    }
}

void Uart::init() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);
    GPIOA->MODER &= ~(GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0);      // 1:0 alt func
    GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEEDR2 | GPIO_OSPEEDR_OSPEEDR3); // max speed
    GPIOA->OTYPER |= GPIO_OTYPER_OT_2;                                 // Tx as open drain
    // GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0; // 0:1 - pull-up
    GPIOA->AFR[0] |= (1 << 8) | (1 << 12); // AF1 (uart) A2 A3
    /*!< UART1 init >*/
    // USART2->CR3 |= USART_CR3_HDSEL; // for true one wire (one for ground and one for data+power)
    USART2->CR1 &= ~USART_CR1_UE;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    USART2->CR1 &= ~USART_CR1_M; // 8 bits
    // Transmition receiving enabled USART2->BRR=0x22B; baudrate=115200 (APB2CLK=64МHz - baudrate/2/baudrate
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
    USART2->BRR = 0xD04;             // baudrate=9600 (APB1CLK=32МHz - baudrate/2)/baudrate
    USART2->CR1 |= USART_CR1_RXNEIE; ////receive not empty interrupt enabled
    USART2->CR1 |= USART_CR1_UE;     // USART1 enable
    NVIC_EnableIRQ(USART2_IRQn);     // received DMA wont work without this interrupt
}

extern "C" void USART2_IRQHandler(void) {
    if (USART2->ISR & USART_ISR_RXNE) {
        Uart::pThis->receivedByte = USART2->RDR;
    }
}