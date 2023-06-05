/*
* ģ������ :�����ʱ�����ܡ�
* �ļ����� : xxxx.h
* ˵    �� :1.��װ������ע��ӿ�;
*          2.��װ����ʱ���нӿڡ�
*
*/
#ifndef _APP_SOFTTIMER_H_
#define _APP_SOFTTIMER_H_

#include "../driver/GMtypes.h"
#include "counter.h"
#include "../driver/config.h"



// �����ʱ��֧������(1~255)
#define SOFTTIMER_CFG_ITEMS_NUMS                    CFG_SOFTTIMER_NUMS

// ��ȡϵͳʱ���(units:1ms)
#define GET_TICK_STAMP()                            counter_get_uptime()


// �����ʱ������ģʽ
typedef enum
{
    E_RUNMODE_NOT_USE = 0,                          // �����ʱ����δʹ��
    E_RUNMODE_SINGLE,                               // �����ʱ��������ģʽ
    E_RUNMODE_CONTINUE ,                            // �����ʱ��������ģʽ
}run_mode_t;


typedef void (*softtimer_cb_t)(void);





// =================== external function ===================
void softtimer_init(void);
void softtimer_register(uint16_t period_ms, softtimer_cb_t pfunc, run_mode_t run_mode);
void softtimer_remove_by_function(softtimer_cb_t function);
void softtimer_poll(void);
void task(void);
uint8_t KeyMonitor(uint8_t GpioKey);
uint8_t KeyHandle();
void app_iris_poll();
void key();
//void filter_oold_poll();
//void filter_poll();
//void filter_ctrl_poll(void);
//void old();
//void filter_res();
void app_df_gpio_init(void);
void app_df_init(void);
void df_ctrl_poll(void);
void df_poll();
void df_pi();
void filter_res();
/*
* ��װ�ӿڣ���������
*/
// ������ȳ�ʼ��
#define TASK_INIT()                                 softtimer_init()
// ���񴴽���(period_ms-����Ϊ����*10)
// ������2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71...
#define TASK_PERIOD_CB_CREATE(period_ms, func)      softtimer_register(period_ms, func, E_RUNMODE_CONTINUE)
// �������٣�ͨ��������
#define TASK_PERIOD_CB_DESTROY(func)                softtimer_remove_by_function(func)
// ������ȣ���ʼ
#define TASK_START()                                counter_init()
// ������ȣ�����
#define TASK_SCHEDULE()                             softtimer_poll()



/*
* ��װ�ӿڣ���ʱ����
* ˵����1.һ��������ʱִ��ĳ������(function)��
*      2.��ʱ����ֻ֧�ֵ���������
*/
// ��ʱ���У�ѹ�������
#define DELAY_QUEUE_PUSH(delay_ms, func)            softtimer_register(delay_ms, func, E_RUNMODE_SINGLE)
// ��ʱ���У��Ӷ����е���
// �����ʱ������ѹ������ͬ�������ģ��򵯳�ֻ��������ѹ��Ĵ�������
#define DELAY_QUEUE_POP_BY_FUNCTION(func)           softtimer_remove_by_function(func)






#endif /* _APP_SOFTTIMER_H_ */
