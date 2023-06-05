/*
 * IR.c
 *
 * Created: 2023/3/7 
 *  Author: zongqiao.pei
 */ 

#include "n32g43x.h"                    // Device header
#include "IR.h"
#include "utils.h"
#include "string.h"

void InputExtiInit(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    /* Enable the GPIO Clock */
    if (GPIOx == GPIOA)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_AFIO, ENABLE);
    }
    else if (GPIOx == GPIOB)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);
    }
    else if (GPIOx == GPIOC)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOC | RCC_APB2_PERIPH_AFIO, ENABLE);
    }
    else
    {
        if (GPIOx == GPIOD)
        {
            RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD | RCC_APB2_PERIPH_AFIO, ENABLE);
        }
    }
    /*Configure the GPIO pin as input floating*/
    if (Pin <= GPIO_PIN_ALL)
    {
        GPIO_InitStruct(&GPIO_InitStructure);
        GPIO_InitStructure.Pin        = Pin;
        GPIO_InitStructure.GPIO_Pull    = GPIO_No_Pull;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Input;
        GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);
    }
    /*Configure key EXTI Line to key input Pin*/
    GPIO_ConfigEXTILine(KEY_INPUT_PORT_SOURCE, KEY_INPUT_PIN_SOURCE);

    /*Configure key EXTI line*/
    EXTI_InitStructure.EXTI_Line    = KEY_INPUT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);

    /*Set key input interrupt priority*/
    NVIC_InitStructure.NVIC_IRQChannel                   = KEY_INPUT_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
} 
uint16_t HW()
{
	uint16_t t = 0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_PIN_2)==1)
	{
		__delay_cycles(20);//20us
		t++;
		if(t>=250) 
		{
			return t;
		}
	}	
	return t;
}
uint32_t hw_jsm;
uint8_t hw_jsbz;

void EXTI2_IRQHandler(void)
{
	uint16_t Tim=0,OK=0,Data,Num=0;
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_PIN_2)==1)
		{
			Tim = HW();
			if(Tim>=250) break;
			if(Tim>=200&&Tim<250)
			{
				OK=1;
			}
			else if(Tim>=60&&Tim<90)
			{
				Data = 1;
			}
			else if(Tim>=10&&Tim<50)
			{
				Data = 0;
			}
			if(OK==1)
			{
				hw_jsm<<=1;
				hw_jsm+=Data;
			if(Num>=32)
			{
				hw_jsbz = 1;
				break;
			}
		}
		Num++;
		}
	}
	EXTI_ClrStatusFlag(EXTI_LINE2);
}



