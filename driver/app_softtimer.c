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
// �����ʱ�����ݽṹ
typedef struct
{
    uint16_t period;                                // ��������
    counter_time_t last_time;                       // �ϴ�ʱ���
    softtimer_cb_t cb;                              // �ص�����
    run_mode_t run_mode;                            // ����ģʽ
}softtimer_t;

static softtimer_t softtimer_table[SOFTTIMER_CFG_ITEMS_NUMS] = {0};

/*
* @����˵��:�����ʱ����ʼ��
* @ע    ��:��
* @��    ��:��
* @��    ��:��
*/
void softtimer_init(void)
{
    // ��ʵ��
}

/*
* @����˵��: �����ʱ����ע��ص��������ṩ���κ�����������ģʽ
* @ע    ��: 1.�����̰߳�ȫ�ģ��������ڶ��̡߳��ж��С�
*           2.�ص�ʱ����Ƚϵͣ�����Ҫ�߾��ȵĽ���ʹ��Ӳ����
* @��    ��: [in]period_ms:ʵ��ms������Χ1~65530ms�����ԣ�COUNTER_TICK_PERIOD_MS��
*                         Ϊ������������ΪCOUNTER_TICK_PERIOD_MS���������������Ͻ�10ms��ʽ
*           [in]run_mode:����ģʽ����������/��������
* @��    ��: ��
*/
void softtimer_register(uint16_t period_ms, softtimer_cb_t pfunc, run_mode_t run_mode)
{
    U16 i;

    for(i=0; i<SOFTTIMER_CFG_ITEMS_NUMS; i++)
    {
        // Ѱ��δʹ�õ�
        if(E_RUNMODE_NOT_USE == softtimer_table[i].run_mode){
            break;
        }
    }

    softtimer_table[i].run_mode = run_mode;
    // ��ʼ��Ϊע��ʱ�̵�ʱ�䡣
    softtimer_table[i].last_time = GET_TICK_STAMP();
    // ����COUNTER_TICK_MS���������㡣
    softtimer_table[i].period = (0 != (period_ms%COUNTER_TICK_PERIOD_MS)) ? \
        period_ms-(period_ms%COUNTER_TICK_PERIOD_MS)+(COUNTER_TICK_PERIOD_MS) : period_ms;
    softtimer_table[i].cb = pfunc;
}

/*
* @����˵��:ͨ�����������Ƴ��Ѿ�ע�����
* @ע    ��:����MCU��Դ���˴����ú�������ΪΨһ��׼�жϣ���ע����2�������ϵ���ͬ������ʱ�����ܴ�����
* @��    ��:[in]function
* @��    ��:��
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
* @����˵��:�����ʱ����ѯ
* @ע    ��:��
* @��    ��:��
* @��    ��:��
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
