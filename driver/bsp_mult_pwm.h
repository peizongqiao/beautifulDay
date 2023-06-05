#ifndef _BSP_MULT_PWM_H_
#define _BSP_MULT_PWM_H_

#include "n32g43x.h"

#define PWM_CHANNEL_NOTHING                      0
#define PWM_CHANNEL_TIM2_CH4                     1        // PWMͨµÀ0
#define PWM_CHANNEL_TIM3_CH2                     2        // PWMͨµÀ1
#define PWM_CHANNEL_TIM4_CH1                     3        // PWMͨµÀ2
#define PWM_CHANNEL_TIM4_CH2                     4        // PWMͨµÀ3
#define PWM_CHANNEL_TIM4_CH3                     5        // PWMͨµÀ4
#define PWM_CHANNEL_TIM4_CH4                     6        // PWMͨµÀ5
#define PWM_CHANNEL_TIM8_CH1                     7        // PWMͨµÀ5
#define PWM_CHANNEL_TIM8_CH2                     8        // PWMͨµÀ5
#define PWM_CHANNEL_TIM8_CH3                     9        // PWMͨµÀ5
#define PWM_CHANNEL_TIM8_CH4                     10       // PWMͨµÀ5
#define PWM_CHANNEL_TIM9_CH3                     11       // PWMͨµÀ5
#define PWM_CHANNEL_TIM9_CH4                     12       // PWMͨµÀ5

typedef uint8_t pwm_channel_t;


void pwm_filter();
void PWM_FAN_1(void);
void bsp_pwm_set_dutycycle(pwm_channel_t channel, uint8_t dutycycle);
void bsp_pwm_set_dutycycle_TIM1_8(pwm_channel_t channel, uint8_t dutycycle);
#endif /* _BSP_MULT_PWM_H_ */
