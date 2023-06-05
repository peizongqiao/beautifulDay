#include <driver_init.h>
#include "counter.h"
#include "app_log.h"
#include "n32g43x.h"

OCInitType COUN_TIM_OCInitStructure;

// 用于系统ticks计数
static volatile counter_time_t counter_uptime = 0;
unsigned int time_nmb = 0; 

void counter_init()
{
    TIM_TimeBaseInitType  TIM_TimeBaseStructure;
    NVIC_InitType NVIC_InitStructure;

    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM1, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    //设置优先级
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

    //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.Prescaler = 80 - 1;
    //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.Period    = 10 - 1;
    //TIM向上计数模式
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    //设置时钟分割
    TIM_TimeBaseStructure.ClkDiv    = 0;
		
		TIM_TimeBaseStructure.RepetCnt  = 0;
		
    TIM_InitTimeBase(TIM1, &TIM_TimeBaseStructure);

    /* 定时器TIM1向上计时 */
    TIM_ConfigInt(TIM1, TIM_INT_UPDATE, ENABLE);
    /* 使能定时器TIM1 */
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
	if(TIM1->STS&0X0001)//溢出中断
	{
		/***************在此处添加中断执行内容******************///进入一次112us  
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
    TIM1->STS&=~(1<<0);//清除中断标志位  
} 

/*
* @功能说明:返回系统
* @注    意:无
* @形    参:无
* @返    回:系统启动后运行时间,类似linux的uptime。 units:1ms
*/


counter_time_t counter_get_uptime(void)
{
    return counter_uptime;
}



/**
  * 得到时间差
  * @return    当前时间
  * @param     前面的时间  新的时间
  * @exception
  **/
counter_time_t get_tim_delta(counter_time_t old, counter_time_t now)
{
    return (counter_time_t)(now - old);
}
