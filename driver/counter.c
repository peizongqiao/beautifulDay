#include <driver_init.h>
#include "counter.h"
#include "app_log.h"
#include "n32g43x.h"

OCInitType COUN_TIM_OCInitStructure;

// ����ϵͳticks����
static volatile counter_time_t counter_uptime = 0;
unsigned int time_nmb = 0; 

void counter_init()
{
    TIM_TimeBaseInitType  TIM_TimeBaseStructure;
    NVIC_InitType NVIC_InitStructure;

    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM1, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    //�������ȼ�
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

    //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.Prescaler = 80 - 1;
    //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.Period    = 10 - 1;
    //TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    //����ʱ�ӷָ�
    TIM_TimeBaseStructure.ClkDiv    = 0;
		
		TIM_TimeBaseStructure.RepetCnt  = 0;
		
    TIM_InitTimeBase(TIM1, &TIM_TimeBaseStructure);

    /* ��ʱ��TIM1���ϼ�ʱ */
    TIM_ConfigInt(TIM1, TIM_INT_UPDATE, ENABLE);
    /* ʹ�ܶ�ʱ��TIM1 */
    TIM_Enable(TIM1, ENABLE);
bool flag_1ms = 0;
}
uint16_t time1_counter1 = 0;
//uint16_t time1_counter2 = 0;
uint16_t time1_counter3 = 0;
uint16_t time1_counter2 = 0;
//bool flag_1ms = 0;
bool flag_20ms = 0;
bool flag_700us = 0;
bool flag_2s = 0;
void TIM1_UP_IRQHandler(void) 
{ 	    	  	     
	if(TIM1->STS&0X0001)//����ж�
	{
		/***************�ڴ˴�����ж�ִ������******************///����һ��112us  
		time1_counter1++;
		time1_counter3++;
		time1_counter2++;
		if(time1_counter1 ==500)//1ms
		{
			time1_counter1 = 0;
			flag_700us = 1;	
		}
		if(time1_counter3 == 2000)//20ms
		{
			time1_counter3 = 0;
			flag_20ms = 1;	
		}
	}	
    TIM1->STS&=~(1<<0);//����жϱ�־λ  
} 

/*
* @����˵��:����ϵͳ
* @ע    ��:��
* @��    ��:��
* @��    ��:ϵͳ����������ʱ��,����linux��uptime�� units:1ms
*/


counter_time_t counter_get_uptime(void)
{
    return counter_uptime;
}



/**
  * �õ�ʱ���
  * @return    ��ǰʱ��
  * @param     ǰ���ʱ��  �µ�ʱ��
  * @exception
  **/
counter_time_t get_tim_delta(counter_time_t old, counter_time_t now)
{
    return (counter_time_t)(now - old);
}
