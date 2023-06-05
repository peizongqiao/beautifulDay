#ifndef _BSP_PI_CFG_H_
#define _BSP_PI_CFG_H_


//ADC0	PA5	通道为AIN5
#define FOCUS_PI        ADC_CH_2_PA1
//ADC0	PA5	通道为AIN5
#define ZOOM_PI         ADC_CH_3_PA2
//ADC0	PA5	通道为AIN5
#define VALVE_PI        ADC_CH_4_PA3
//ADC0	PA5	通道为AIN5
#define IRIS_PI         ADC_CH_5_PA4


//PI低电平检测限值电压
#define PI_V		0.2
//采样电压误差为0.1
#define PI_DELTA	0.1

#endif /* _BSP_PI_CFG_H_ */