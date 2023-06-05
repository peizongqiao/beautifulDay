#include "app_softtimer.h"
#include "../driver/GMtypes.h"
#include "../driver/utils.h"
#include "driver_init.h"
#include "app_log.h"
#include "app_focal_ctrl.h"
#include "main.h"
#include "i2c_slave_N32G43X.h"


extern bool flag_20ms;
extern bool flag_700us;
extern uint8_t R_flag;
// 软件定时器数据结构
typedef struct
{
    uint16_t period;                                // 运行周期
    counter_time_t last_time;                       // 上次时间戳
    softtimer_cb_t cb;                              // 回调函数
    run_mode_t run_mode;                            // 运行模式
}softtimer_t;

static softtimer_t softtimer_table[SOFTTIMER_CFG_ITEMS_NUMS] = {0};

/*
* @功能说明:软件定时器初始化
* @注    意:无
* @形    参:无
* @返    回:无
*/
void softtimer_init(void)
{
    // 空实现
}

/*
* @功能说明: 软件定时任务，注册回调函数，提供单次和周期性两种模式
* @注    意: 1.不是线程安全的，不能用于多线程、中断中。
*           2.回调时间进度较低，如需要高精度的建议使用硬件。
* @形    参: [in]period_ms:实际ms数，范围1~65530ms。是以（COUNTER_TICK_PERIOD_MS）
*                         为整倍数；若不为COUNTER_TICK_PERIOD_MS整数倍，采用向上进10ms形式
*           [in]run_mode:运行模式，连续运行/单次运行
* @返    回: 无
*/
void softtimer_register(uint16_t period_ms, softtimer_cb_t pfunc, run_mode_t run_mode)
{
    U16 i;

    for(i=0; i<SOFTTIMER_CFG_ITEMS_NUMS; i++)
    {
        // 寻找未使用的
        if(E_RUNMODE_NOT_USE == softtimer_table[i].run_mode){
            break;
        }
    }

    softtimer_table[i].run_mode = run_mode;
    // 初始化为注册时刻的时间。
    softtimer_table[i].last_time = GET_TICK_STAMP();
    // 不足COUNTER_TICK_MS倍数，补足。
    softtimer_table[i].period = (0 != (period_ms%COUNTER_TICK_PERIOD_MS)) ? \
        period_ms-(period_ms%COUNTER_TICK_PERIOD_MS)+(COUNTER_TICK_PERIOD_MS) : period_ms;
    softtimer_table[i].cb = pfunc;
}

/*
* @功能说明:通过函数名称移除已经注册服务。
* @注    意:限于MCU资源，此处采用函数名作为唯一标准判断，当注册了2个及以上的相同处理函数时，可能存在误差。
* @形    参:[in]function
* @返    回:无
*/
void softtimer_remove_by_function(softtimer_cb_t function)
{
    for(uint8_t i=0; i<SOFTTIMER_CFG_ITEMS_NUMS; i++)
    {
        if(E_RUNMODE_NOT_USE != softtimer_table[i].run_mode){
            if(function == softtimer_table[i].cb){
                softtimer_table[i].run_mode = E_RUNMODE_NOT_USE;
                return;
            }
        }
    }
}

/*
* @功能说明:软件定时器轮询
* @注    意:无
* @形    参:无
* @返    回:无
*/
void softtimer_poll(void)
{
    for (uint8_t i = 0; i < SOFTTIMER_CFG_ITEMS_NUMS; i++)//SOFTTIMER_CFG_ITEMS_NUMS = 15
    {
        softtimer_t *p_timer = &softtimer_table[i];

        if (E_RUNMODE_NOT_USE == p_timer->run_mode) {
            continue;
        }
        if (get_tim_delta(p_timer->last_time, GET_TICK_STAMP()) >= p_timer->period){
            p_timer->cb();
            if (E_RUNMODE_SINGLE == p_timer->run_mode){
                p_timer->run_mode = E_RUNMODE_NOT_USE;
                continue;
            }
            p_timer->last_time = GET_TICK_STAMP();
        }
    }
}



uint8_t count;

void task(void)
{
	if(flag_20ms)
	{
		count++;
		if(count==2 == 1)
		{
			HC595SendData(0x05);
		}
		else if(count == 2)
		{
			HC595SendData(0x06);
		}
		else if(count == 3)
		{
			HC595SendData(0x07);
		}
		else if(count == 4)
		{
			HC595SendData(0x08);
		}
		else if(count == 5)
		{
			HC595SendData(0x09);
		}
				
			//		log_info("n40.val=%d\xff\xff\xff",r_current);
			//		log_info("n41.val=%d\xff\xff\xff",g_current);
			//		log_info("n42.val=%d\xff\xff\xff",b_current);
			//		
			//		log_info("n5.val=%d\xff\xff\xff",R_FAN_SPEED);
			//		log_info("n6.val=%d\xff\xff\xff",G_FAN_SPEED);
			//		log_info("n7.val=%d\xff\xff\xff",B_FAN_SPEED);
			//		log_info("n8.val=%d\xff\xff\xff",BP_FAN_SPEED);
			//		log_info("n9.val=%d\xff\xff\xff",DR_FAN_SPEED);
			//		log_info("n3.val=%d\xff\xff\xff",BP_current);
			//		log_info("n0.val=%d\xff\xff\xff",DR_current);
			//		log_info("x0.val=%d\xff\xff\xff",R_temp);
			//		log_info("x1.val=%d\xff\xff\xff",G_temp);
			//		log_info("x2.val=%d\xff\xff\xff",B_temp);
			//		log_info("x3.val=%d\xff\xff\xff",BP_temp);
			//		log_info("x4.val=%d\xff\xff\xff",DR_temp);
			//		log_info("x5.val=%d\xff\xff\xff",TEMP_DMD);


			flag_20ms = 0;
	}
	if(flag_700us)
	{

		flag_700us = 0;
	}
}
