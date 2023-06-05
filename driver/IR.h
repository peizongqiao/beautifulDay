/*
 * IR.h
 *
 * Created: 2019/7/31 22:12:08
 *  Author: Ade.tang
 */ 


#ifndef IR_H_
#define IR_H_
#include <compiler.h>
#define KEY_INPUT_PORT        GPIOB
#define KEY_INPUT_PIN         GPIO_PIN_2
#define KEY_INPUT_EXTI_LINE   EXTI_LINE2
#define KEY_INPUT_PORT_SOURCE GPIOB_PORT_SOURCE
#define KEY_INPUT_PIN_SOURCE  GPIO_PIN_SOURCE2
#define KEY_INPUT_IRQn        EXTI2_IRQn

void InputExtiInit(GPIO_Module* GPIOx, uint16_t Pin);



#endif /* IR_H_ */