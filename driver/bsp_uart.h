/*
* 模块名称 :uart驱动
* 文件名称 :xxxxx.h
* 说    明 :目前只是支持单路配置。
*
*/
#ifndef _BSP_UART_H_
#define _BSP_UART_H_


#include "bsp_uart_cfg.h"


// ======================== external ========================
void bsp_uart_api_init(void);
void bsp_uart_api_init_1();
void HMI_SendNum(USART_Module* USARTx,char *data,u8 len,u16 Num,u8 len2);
u32 HMI_Pow(u8 n);
void HMI_SendText(USART_Module* USARTx,char *data,u8 len,char *text,u8 len2);
uint8_t Usart_Sum_rec( USART_Module * pUSARTx, uint8_t * rev);


#endif