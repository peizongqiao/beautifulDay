#ifndef _BSP_UART_CFG_H_
#define _BSP_UART_CFG_H_

#include "n32g43x.h"
#include "n32g43x_gpio.h"
#include "n32g43x_usart.h"
#include "n32g43x_rcc.h"

#define LCD_USARTx      USART1
#define LCD_PERIPH      RCC_APB2_PERIPH_USART1
#define LCD_GPIO        GPIOA
#define LCD_PERIPH_GPIO RCC_APB2_PERIPH_GPIOA
#define LCD_TX_PIN      GPIO_PIN_4
#define LCD_RX_PIN      GPIO_PIN_5
#define LCD_Rx_GPIO_AF  GPIO_AF4_USART1
#define LCD_Tx_GPIO_AF  GPIO_AF1_USART1
#define LCD_APBxClkCmd  RCC_EnableAPB2PeriphClk
#define LCD_IRQn        USART1_IRQn
#define LCD_IRQHandler  USART1_IRQHandler


#endif /* _BSP_UART_CFG_H_ */