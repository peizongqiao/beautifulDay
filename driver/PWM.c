//#include  "pwm_basic.h"
#include "bsp_mult_pwm.h"
//#include "N32G43x_Start.h"


GPIO_InitType PWM_GPIO_InitStructure;
TIM_TimeBaseInitType TIM_TimeBaseStructure;
OCInitType TIM_OCInitStructure;
uint16_t Channel1Pulse = 0,Channel2Pulse = 0,Channel3Pulse = 0,Channel4Pulse = 0;
void PWM_FAN_1(void)
{
GPIO_InitType GPIO_InitStructure;
    //PB6 TIM4_CH1 PB7 TIM4_CH2 PB8 TIM4_CH3 PB9 TIM4_CH4
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM4, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);  

	GPIO_InitStruct(&GPIO_InitStructure);
    /* GPIOA Configuration: Channel 1, 2 and 3 as alternate function push-pull */
    GPIO_InitStructure.Pin        = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM4;
    GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);


     //´¥·¢ʱ¼�?Prescaler + 1) * (Period + 1)/54M ¼򻯎ª2*Prescaler/
    //ɨփӃÀ´׷ΪTIMxʱ֓Ƶ³ýʽµĔ¤·ֆµֵ
    TIM_TimeBaseStructure.Prescaler = 10 - 1;
     //ɨփԚςһ¸ö¸üЂʂ¼þװȫ»µėԶ¯֘װԘ¼Ĵ憷֜ƚµĖµ
     TIM_TimeBaseStructure.Period    = 200;
     //TIMϲɏ¼Ɗýģʽ
     TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    //ɨփʱ֓·ָ�?   TIM_TimeBaseStructure.ClkDiv    = 0;    
    TIM_InitTimeBase(TIM4, &TIM_TimeBaseStructure); 
    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.OcMode       = TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse        = 118;
    TIM_OCInitStructure.OcPolarity   = TIM_OC_POLARITY_LOW;    
    TIM_InitOc1(TIM4, &TIM_OCInitStructure);    
    TIM_OCInitStructure.Pulse = 118;
    TIM_InitOc2(TIM4, &TIM_OCInitStructure);
    TIM_OCInitStructure.Pulse = 50;
    TIM_InitOc3(TIM4, &TIM_OCInitStructure);
    TIM_OCInitStructure.Pulse = 50;
    TIM_InitOc4(TIM4, &TIM_OCInitStructure);

    TIM_ConfigArPreload(TIM4, ENABLE);  
    TIM_Enable(TIM4, ENABLE);
	
}

void pwm_filter()
{
    //PC6 TIM8_CH1 PC7 TIM8_CH2 PC8 TIM8_CH3 PC9 TIM8_CH4
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOC | RCC_APB2_PERIPH_AFIO | RCC_APB2_PERIPH_TIM8, ENABLE);  
    DBG_ConfigPeriph(DBG_TIM8_STOP , ENABLE);
		
		GPIO_InitType GPIO_InitStructure;

    GPIO_InitStruct(&GPIO_InitStructure);
    /* GPIOA Configuration: Channel 1, 2 and 3 as alternate function push-pull */
    GPIO_InitStructure.Pin        = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF6_TIM8;
    GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
    GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);
	
    Channel1Pulse = (uint16_t)(((uint32_t)99 * (200 - 1)) / 100);
    Channel2Pulse = (uint16_t)(((uint32_t)99 * (200 - 1)) / 100);
    Channel3Pulse = (uint16_t)(((uint32_t)99 * (200 - 1)) / 100);
    Channel4Pulse = (uint16_t)(((uint32_t)99 * (200 - 1)) / 100);

     //´¥·¢ʱ¼�?Prescaler + 1) * (Period + 1)/108M
    //ɨփӃÀ´׷ΪTIMxʱ֓Ƶ³ýʽµĔ¤·ֆµֵ
    TIM_TimeBaseStructure.Prescaler = 10 - 1;
     //ɨփԚςһ¸ö¸üЂʂ¼þװȫ»µėԶ¯֘װԘ¼Ĵ憷֜ƚµĖµ
     TIM_TimeBaseStructure.Period    = 108 - 1;
     //TIMϲɏ¼Ɗýģʽ
     TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    //ɨփʱ֓·ָ�?   TIM_TimeBaseStructure.ClkDiv    = 0; 
    TIM_TimeBaseStructure.RepetCnt  = 0;   
    TIM_InitTimeBase(TIM8, &TIM_TimeBaseStructure); 
    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.OcMode       = TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse        = 50;
    TIM_OCInitStructure.OcPolarity   = TIM_OC_POLARITY_LOW;    
    TIM_InitOc1(TIM8, &TIM_OCInitStructure);    
    TIM_OCInitStructure.Pulse = 50;
    TIM_InitOc2(TIM8, &TIM_OCInitStructure);
    TIM_OCInitStructure.Pulse = 50;
    TIM_InitOc3(TIM8, &TIM_OCInitStructure);
    TIM_OCInitStructure.Pulse = 50;
    TIM_InitOc4(TIM8, &TIM_OCInitStructure);

    //TIM_ConfigArPreload(TIM8, ENABLE);  
    TIM_Enable(TIM8, ENABLE);
    TIM_EnableCtrlPwmOutputs(TIM8, ENABLE);
}
