#include "app_focal_ctrl.h"
#include "bsp_focal_dz24.h"
#include "app_softtimer.h"
#include "app_log.h"
#include "bsp_pi_api.h"
#include "app_iris.h"
#include "bsp_filtermotor_soft_subdivision.h"


uint16_t flag_34;
uint16_t focal_old=3000,focal_pps;
uint16_t zoom_old=3000;
uint16_t focal_step=0;
uint16_t zoom_step=0;
uint8_t focus_step_number;
uint8_t zoom_step_number;
uint8_t focal_end = 0,zoom_end = 0,focal_pi_flag = 0,zoom_pi_flag = 0;
bool RecycleFlag = false;
bool RecycleFlag_1 = false;
bool RecycleFlag_2 = false;
uint16_t RcycleCnt_focus = 0;
uint16_t RcycleCnt_zoom = 0;
uint16_t RcycleCnt_filter = 0;


void focal_init(void)
{
    focal_ctrl_t *pctrl = &focal_ctrl;

    pctrl->focus_run_number = 0;
	pctrl->zoom_run_number = 0;
    pctrl->focus_direction = E_DIRECTION_STOP;
    pctrl->zoom_direction = E_DIRECTION_STOP;
    bsp_focal_api_init();
}

void old(void)
{
	focal_ctrl_t *pctrl = &focal_ctrl;
	float Key;
	Key = (float)ADC_GetData(ADC_CH_14_PC3)*3.3/1023;
	if((1.8<Key) && (Key<2.02)){// OLD
	    RecycleFlag = true;
		RecycleFlag_1 = true;
		RecycleFlag_2 = true;
	}
	else if ((2.9<Key) && (Key<3.1)) // Manual
	{
		pctrl -> focus_direction = E_DIRECTION_STOP;
		pctrl -> zoom_direction = E_DIRECTION_STOP;
		pctrl -> filter_direction = E_DIRECTION_STOP;
		RecycleFlag = false;
		RecycleFlag_1 = false;
		RecycleFlag_2 = false;
	}
}

 void focal_ctrl_poll(void)
{
	focal_ctrl_t *pctrl = &focal_ctrl;
	float Key;
	Key = (float)ADC_GetData(ADC_CH_14_PC3)*3.3/1023;
	if((2.3<Key) && (Key<2.5))
	{
		pctrl -> focus_direction = E_DIRECTION_RET;
		focal_step=20;
	}
	else if((2.6<Key) && (Key<2.8))
	{
		pctrl -> focus_direction = E_DIRECTION_RUN;
		focal_step=20;
	}
}		
 void focal_diver_poll(void)
{
	focal_ctrl_t *pctrl = &focal_ctrl;
	focal_pi_flag = false;
	if(RecycleFlag==false)
	{
		if(true == focal_pi_flag)
		{
			focal_step = 0;
			focal_end = 1;
			bsp_focus_api_stop();
		}
		else if(focal_step != 0)
		{
			pctrl -> focus_run_number++;
			if(pctrl -> focus_run_number >= 8)
				pctrl -> focus_run_number = 0;
			bsp_focus_api_run(pctrl ->focus_run_number,pctrl ->focus_direction);
			if(E_DIRECTION_RUN == pctrl ->focus_direction)
				focus_step_number++;
			else if(E_DIRECTION_RET == pctrl ->focus_direction)
				focus_step_number--;
			focal_step --;
			if(focal_step == 0)
			{
				focal_end = 1;
			}
		}
	}
	else if(RecycleFlag==true)
	{
		if(RcycleCnt_focus < 3600)
		{
			RcycleCnt_focus++;
		}
		else
		{				
			RcycleCnt_focus = 0;
			if(pctrl ->focus_direction==E_DIRECTION_RUN)
				pctrl ->focus_direction = E_DIRECTION_RET;
			else
				pctrl ->focus_direction = E_DIRECTION_RUN;
		}
		pctrl -> focus_run_number++;
		if(pctrl -> focus_run_number >= 8)
			pctrl -> focus_run_number = 0;
		bsp_focus_api_run(pctrl ->focus_run_number,pctrl ->focus_direction);
		if(E_DIRECTION_RUN == pctrl ->focus_direction)
			focus_step_number++;
		else if(E_DIRECTION_RET == pctrl ->focus_direction)
			focus_step_number--;
	}
}
 void zoom_ctrl_poll(void)
{
	focal_ctrl_t *pctrl = &focal_ctrl;
	float Key;
	Key = (float)ADC_GetData(ADC_CH_14_PC3)*3.3/1023;
	if((0.9<Key) && (Key<1.1))//if((2.9<i) && (i<3.1))
	{
		pctrl -> zoom_direction = E_DIRECTION_RET;
		zoom_step=20;
	}
	else if((2.1<Key) && (Key<2.3))//if((2.02<i) && (i<2.22))
	{
		pctrl -> zoom_direction = E_DIRECTION_RUN;
		zoom_step=20;
	}
}

void zoom_diver_poll(void)
{
	focal_ctrl_t *pctrl = &focal_ctrl;
	zoom_pi_flag = false;
	if(RecycleFlag_1==false)
	{
		if(true == zoom_pi_flag)
		{
			zoom_step = 0;
			zoom_end = 1;
			bsp_zoom_api_stop();
		}
		else if(zoom_step != 0)
		{
			pctrl -> zoom_run_number++;
			if(pctrl -> zoom_run_number >= 8)
				pctrl -> zoom_run_number = 0;
			bsp_zoom_api_run(pctrl ->zoom_run_number,pctrl ->zoom_direction);
			if(E_DIRECTION_RUN == pctrl ->zoom_direction)
				zoom_step_number++;
			else if(E_DIRECTION_RET == pctrl ->zoom_direction)
				zoom_step_number--;
			zoom_step --;
			if(zoom_step == 0)
			{
				zoom_end = 1;
			}
		}
	}
	else
	{
		if(RcycleCnt_zoom < 3600)
		{
			RcycleCnt_zoom++;
		}
		else
		{				
			RcycleCnt_zoom = 0;
			if(pctrl ->zoom_direction==E_DIRECTION_RUN)
				pctrl ->zoom_direction = E_DIRECTION_RET;
			else
				pctrl ->zoom_direction = E_DIRECTION_RUN;
		}
		pctrl -> zoom_run_number++;
		if(pctrl -> zoom_run_number >= 8)
			pctrl -> zoom_run_number = 0;
		bsp_zoom_api_run(pctrl ->zoom_run_number,pctrl ->zoom_direction);
	}
}

uint16_t FILTER_CLOSE = 0;
uint16_t FLAG_OPEN = 0;
bool flag = 0;
uint16_t filter_step_close=0;
uint16_t filter_step=0;
uint16_t filter_step_half=0;
uint8_t  filter_pi_flag;
uint16_t flag_half=0;
uint16_t filter_step_open=0;
bool filter_reset;

void filter_res()
{
	filter_reset = FocusMotorWait();
	if(filter_reset==0)
	{
		filter_step_close=3600;
		filter_step=0;
		filter_step_half=0;
		flag=0;
		FLAG_OPEN = 0;
		flag_half = 0;
	}
}

void filter_ctrl_poll(void)
{
		filter_pi_flag = FocusMotorWait();
		focal_ctrl_t *pctrl = &focal_ctrl;
		float i,k;
	    i = (float)ADC_GetData(ADC_CH_1_PA0)*3.3/1023;
	    k = (float)ADC_GetData(ADC_CH_14_PC3)*3.3/1023;
		if((1.38<i) && (i<1.58))
		{
			filter_step=3560;
			flag = 1;
		}
		else if((2.3<i) && (i<2.5))//(2.3<Key) && (Key<2.5)
		{
			filter_step_close=3600;
			filter_step=0;
			filter_step_half=0;
			flag=0;
			FLAG_OPEN = 0;
			flag_half = 0;
		}
		else if((1.5<k) && (k<1.7))
		{
			filter_step_half=2360;
			flag = 1;
		}
}	

void filter_poll()
{
	if(filter_pi_flag == 1&&flag==0)
	{
		bsp_filtermotor_api_stop();
	}
	else
	{
		if(filter_step_close!=0&&flag==0)//K11
		{
			bsp_filtermotor_api_run(10000, 1, E_DIRECT_CLOSE);
			filter_step_close--;
		}
		else if(FLAG_OPEN<3560&&filter_step!=0)//K12
		{
			FLAG_OPEN++;
			bsp_filtermotor_api_run(10000, 1, E_DIRECT_OPEN);
			filter_step--;
		}
		else if(flag_half <2360&&filter_step_half!=0)//K13
		{
			flag_half++;
			bsp_filtermotor_api_run(10000, 1, E_DIRECT_OPEN);
			filter_step_half--;
		}
	}
}
bool DIR = 0;
void filter_oold_poll()
{
	if(RecycleFlag_2==true)
	{
		if(RcycleCnt_filter < 3560)
			{
				RcycleCnt_filter++;
			}
			else
			{				
				RcycleCnt_filter = 0;
				if(DIR==0)
					DIR=1;
				else
					DIR=0;
			}
			bsp_filtermotor_api_run(10000,1,DIR);
	}
}




