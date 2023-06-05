/*
 * control_task.h
 *
 * Created: 2018/5/2 9:39:34
 *  Author: TF
 */ 


#ifndef CONTROL_TASK_H_
#define CONTROL_TASK_H_
#include "../driver/utils.h"
typedef struct  task_components
{
	uint8_t run_flag;//�������б�־
	uint16_t timer;//��ʱ��
	uint16_t itv_time;//������ʱ��
	void (* task_hook)(void);//������ָ��
	struct list_head qset;
}task_componets_t;

void task_init(void);
void task_add(void (*task_hook)(void), uint16_t itv_time);
void task_process(void);
void task_remarks(void);

#endif /* CONTROL_TASK_H_ */