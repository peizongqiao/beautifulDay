#include "app_softtimer.h"
#include "app_log.h"
#include "bsp_pi_api.h"
#include "bsp_filtermotor_soft_subdivision.h"
#include "main.h"



extern uint8_t flag_df;
bool RecycleFlag_2 = false;
uint16_t RcycleCnt_filter = 0;
uint16_t FILTER_CLOSE = 0;
uint16_t FLAG_OPEN = 0;
bool flag = 0;
uint16_t filter_step_close=0;
uint16_t filter_step=0;
uint16_t filter_step_half=0;
uint8_t  filter_pi_flag;
uint16_t flag_half=0;
uint16_t filter_step_open=0;
extern uint8_t FILTER_PI;

bool filter_reset;

void app_df_init(void)
{
    app_df_gpio_init();
}

void app_df_gpio_init(void)
{
    GPIO_InitType GPIO_InitStructure;
    
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOC, ENABLE);

    GPIO_InitStruct(&GPIO_InitStructure);

    GPIO_InitStructure.Pin        = GPIO_PIN_14;
    GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
    GPIO_InitStructure.GPIO_Pull    = GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
 
		
}
uint16_t df_step;
bool Recycle;
uint16_t df_step_number;
uint32_t Cnt;
bool df_direction;
uint16_t df_step;
bool df_pi_Flag;
void df_pi()
{
	if(FILTER_PI==0&&df_pi_Flag==1)
	{
		df_pi_Flag = 0;
		flag_df = 0;
		df_step = 0;
	}
}

void df_ctrl_poll(void)
{		
	if(flag_df==1) // Zero
	{
		df_pi_Flag=1;
		HC595SendData_1(0x60);
		flag_df=0;
		df_step = 14000;
	}
	if(flag_df==2) // Farward 28 Step
	{
		df_pi_Flag=0;
		HC595SendData_1(0x20);
		flag_df=0;
		df_step = 10500;//476
	}
	if(flag_df==3) // Farward 7 Step
	{
		df_pi_Flag=0;
		HC595SendData_1(0x20);
		flag_df=0;	
		df_step = 7000;
	}
	if(flag_df==4) // Farward 22 Step
	{
		Recycle = true;
		flag_df=0;			
	}
	if(flag_df==5) // Farward 22 Step
	{
		Recycle = false;
		flag_df=0;	
	}

}

void df_poll()
{
	if(df_step!=0&&Recycle == false)
	{
		if(0 == df_step_number)
		{
			df_step_number++;
			GPIOB->PBSC = GPIO_PIN_14;
		}
		else if(1 == df_step_number)
		{
			df_step_number = 0;
			GPIOB->PBC = GPIO_PIN_14;
			df_step--;
		}
	}
	else if(Recycle == true)                                      //¿œªØIRIS
	{
		if(Cnt < 21000)
		{
			Cnt++;
		}
		else
		{				
			Cnt = 0;
			if(df_direction == 1)
			{
				HC595SendData_1(0x20);
				df_direction = 0;
			}
			else
			{
				HC595SendData_1(0x60);
				df_direction = 1;
			}
		}
		if(0 == df_step_number)
		{
			df_step_number++;
			GPIOB->PBSC = GPIO_PIN_14;
		}
		else
		{
			df_step_number = 0;
			GPIOB->PBC = GPIO_PIN_14;
		}
	}
}


void filter_res()
{
	filter_reset = FILTER_PI;
	if(filter_reset==1)
	{
		HC595SendData_1(0x60);
		flag_df=0;
		df_step = 14000;
	}
}

//void old()
//{
//	if(flag_df==4){// OLD
//		flag_df=0;
//		RecycleFlag_2 = true;
//	}
//	else if (flag_df==5) // Manual
//	{
//		flag_df=0;
//		RecycleFlag_2 = false;
//	}
//}
//void filter_ctrl_poll(void)
//{
//	filter_pi_flag = FILTER_PI;
//	if(flag_df==2)//K24
//	{
//		flag_df=0;
//		filter_step=3560;
//		flag = 1;
//	}
//	else if(flag_df==1)//(2.3<Key) && (Key<2.5)K23
//	{
//		flag_df=0;
//		filter_step_close=3600;
//		filter_step=0;
//		filter_step_half=0;
//		flag=0;
//		FLAG_OPEN = 0;
//		flag_half = 0;
//	}
//	else if(flag_df==3)
//	{
//		flag_df=3;
//		filter_step_half=2360;
//		flag = 1;
//	}
//}	

//void filter_poll()
//{
//	if(RecycleFlag_2==false)
//	{
//		if(filter_pi_flag == 0&&flag==0)
//		{
//			bsp_filtermotor_api_stop();
//		}
//		else
//		{
//			if(filter_step_close!=0&&flag==0)//K23
//			{
//				bsp_filtermotor_api_run(10000, 1, E_DIRECT_CLOSE);
//				filter_step_close--;
//			}
//			else if(FLAG_OPEN<3560&&filter_step!=0)//K24
//			{
//				FLAG_OPEN++;
//				bsp_filtermotor_api_run(10000, 1, E_DIRECT_OPEN);
//				filter_step--;
//			}
//			else if(flag_half <2360&&filter_step_half!=0)//K25
//			{
//				flag_half++;
//				bsp_filtermotor_api_run(10000, 1, E_DIRECT_OPEN);
//				filter_step_half--;
//			}
//		}
//	}
//}
//bool DIR = 0;
//void filter_oold_poll()
//{
//	if(RecycleFlag_2 == true)
//	{
//		if(RcycleCnt_filter < 3560)
//			{
//				RcycleCnt_filter++;
//			}
//			else
//			{				
//				RcycleCnt_filter = 0;
//				if(DIR==0)
//					DIR=1;
//				else
//					DIR=0;
//			}
//			bsp_filtermotor_api_run(10000,1,DIR);
//	}
//}


