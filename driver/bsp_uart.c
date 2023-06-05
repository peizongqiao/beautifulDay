#include "bsp_uart.h"
#include "app_log.h"
#include <string.h>

uint8_t RxBuffer[5];
uint8_t log_number = 0,log_flag = 0;

void UART_NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;
    
     /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel            = LCD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void bsp_uart_api_init()
{
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;

    GPIO_InitStruct(&GPIO_InitStructure);

    RCC_EnableAPB2PeriphClk(LCD_PERIPH_GPIO, ENABLE);

    LCD_APBxClkCmd(LCD_PERIPH, ENABLE);

    UART_NVIC_Configuration();
    
    GPIO_InitStructure.Pin        = LCD_TX_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Alternate = LCD_Tx_GPIO_AF;
    GPIO_InitPeripheral(LCD_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.Pin       = LCD_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = LCD_Rx_GPIO_AF;
    GPIO_InitPeripheral(LCD_GPIO, &GPIO_InitStructure);

    USART_InitStructure.BaudRate            = 115200;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

    // init uart
    USART_Init(LCD_USARTx, &USART_InitStructure);

    // enable uart
    USART_Enable(LCD_USARTx, ENABLE);
    
    USART_ConfigInt(LCD_USARTx, USART_INT_RXDNE, ENABLE);
}

void bsp_uart_api_init_1()
{
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;

    GPIO_InitStruct(&GPIO_InitStructure);

    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);

    LCD_APBxClkCmd(RCC_APB2_PERIPH_UART4, ENABLE);

    
    GPIO_InitStructure.Pin        = GPIO_PIN_14;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Alternate = GPIO_AF6_UART4;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.Pin       = GPIO_PIN_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF6_UART4;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.BaudRate            = 115200;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

    // init uart
    USART_Init(UART4, &USART_InitStructure);

    // enable uart
    USART_Enable(UART4, ENABLE);
    
    USART_ConfigInt(UART4, USART_INT_RXDNE, ENABLE);
}

static int is_lr_sent = 0;

int fputc(int ch, FILE* f)
{
    if (ch == '\r')
    {
        is_lr_sent = 1;
    }
    else if (ch == '\n')
    {
        if (!is_lr_sent)
        {
            USART_SendData(LCD_USARTx, (uint8_t)'\r');
            /* Loop until the end of transmission */
            while (USART_GetFlagStatus(LCD_USARTx, USART_FLAG_TXC) == RESET)
            {
            }
        }
        is_lr_sent = 0;
    }
    else
    {
        is_lr_sent = 0;
    }
    USART_SendData(LCD_USARTx, (uint8_t)ch);
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(LCD_USARTx, USART_FLAG_TXC) == RESET)
    {
    }
    return ch;
}


/**
 * @brief  Main program.
 */
//字符接收函数：
//uint8_t Usart_Sum_rec( USART_TypeDef * pUSARTx, uint8_t * rev)
//返回值：1为获取数据成功   0为获取数据失败
//USART_TypeDef * pUSARTx   选择串口输出，注意必须先初始化、使能该串口
//uint8_t * rev  利用指针获取数据
uint8_t Usart_Sum_rec( USART_Module * pUSARTx, uint8_t * rev)
{
   if(USART_GetFlagStatus(LCD_USARTx, USART_FLAG_RXDNE) != RESET)
   {  
     *rev = USART_ReceiveData(pUSARTx);
     return 1;
   }
   return 0;
 }

 //字符串发送函数：
//HMI_SendText(USART_TypeDef* USARTx,u8 *data,u8 len,u8 *text,u8 len2)
//USART_TypeDef* USARTx： 选择串口输出，注意必须先初始化、使能该串口
//char *data：上位机上文本位相应的改变数值代码，如"t0.txt="、"t1.txt="cv
//u8 len：*data代码的长度。如"t0.txt="长度为7。
//char *text：需要串口输出显示的文本
//u8 len2：需要显示的文本的长度，英文一个字符一字节、汉字一个字两个字节    

void HMI_SendText(USART_Module* USARTx,char *data,u8 len,char *text,u8 len2)
{
  u8 t;
  for(t=0;t<len;t++)
  {  
    while(USART_GetFlagStatus(USARTx,USART_FLAG_TXC)!=SET);
    USART_SendData(USARTx,data[t]);          
  }
  while(USART_GetFlagStatus(USARTx,USART_FLAG_TXC)!=SET) {;}       //双引号
  USART_SendData(USARTx,0X22); 
  for(t=0;t<len2;t++)  
  {
    while(USART_GetFlagStatus(USARTx,USART_FLAG_TXC)!=SET);
    USART_SendData(USARTx,text[t]); 
  }
  while(USART_GetFlagStatus(USARTx,USART_FLAG_TXC)!=SET){;}        //双引号
  USART_SendData(USARTx,0X22);        
  for(int count=0;count<3;count++){
    while(USART_GetFlagStatus(USARTx,USART_FLAG_TXC)!=SET);
    USART_SendData(USARTx,0XFF);
  }
}

//10的乘方函数，用于判断位数
u32 HMI_Pow(u8 n)
{
  u32 result=1;   
  while(n--)result*=10;    
  return result;
}  

//数值发送函数
//USART_TypeDef* USARTx： 选择串口输出，注意必须先初始化、使能串口
//char *data：上位机上数字位相应的改变数值代码
//u8 len：*data代码的长度。
//u16 Num：需要显示的数值，注意开头0不显示
//u8 len2：需要显示的数值的位数
void HMI_SendNum(USART_Module* USARTx,char *data,u8 len,u16 Num,u8 len2)
{
  u8 t;
  for(t=0;t<len;t++)
  {  
    while(USART_GetFlagStatus(USARTx,USART_FLAG_TXC)!=SET);
    USART_SendData(USARTx,data[t]);          
  }
  for(t=0;t<len2;t++)  
  {
    while(USART_GetFlagStatus(USARTx,USART_FLAG_TXC)!=SET);
    USART_SendData(USARTx,'0'+(Num/HMI_Pow(len2-t-1))%10);   //转化为字符型+48
  }
  for(int count=0;count<3;count++){
    while(USART_GetFlagStatus(USARTx,USART_FLAG_TXC)!=SET);
    USART_SendData(USARTx,0XFF);
  }
}

/**
 * @brief  This function handles USARTz global interrupt request.
 */
void LCD_IRQHandler(void)
{
	uint8_t BcckData;
    if (USART_GetIntStatus(LCD_USARTx, USART_INT_RXDNE) != RESET)
    {
        USART_ClrIntPendingBit(LCD_USARTx,USART_INT_RXDNE);  
		if(0 == log_flag)
		{
        RxBuffer[log_number] = USART_ReceiveData(LCD_USARTx);     
		log_number++;
		if(log_number==5)
		{
			log_flag=1;
			log_number = 0;
		}
	}
  }
}

#ifdef USE_FULL_ASSERT

__WEAK void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    log_error("assertion failed: `%s` at %s:%d", expr, file, line);
    while (1)
    {
    }
}
#endif // USE_FULL_ASSERT
