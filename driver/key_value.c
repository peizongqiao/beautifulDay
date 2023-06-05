#include "app_softtimer.h"
#include "dlp_manager.h"
#include "adc_basic.h"
#include "i2c_slave_N32G43X.h"
#include "app_log.h"
#define Key11 0x11
#define Key12 0x12
#define Key13 0x13
#define Key14 0x14
#define Key20 0x20
#define Key21 0x21
#define Key22 0x22
#define Key23 0x23
#define Key24 0x24
#define Key25 0x25
#define Key31 0x31
#define Key32 0x32
#define Key33 0x33
#define Key34 0x34
#define Key26 0x26

uint32_t ShakeCnt0 = 0;
uint32_t ShakeCnt1 = 0;
uint8_t PicSwtichCnt = 0;
void LedInit_1(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    /* Enable the GPIO Clock */
    if (GPIOx == GPIOA)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    }
    else if (GPIOx == GPIOB)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    }
    else if (GPIOx == GPIOC)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOC, ENABLE);
    }
    else
    {
        if (GPIOx == GPIOD)
        {
            RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD, ENABLE);
        }
    }

    /* Configure the GPIO pin */
    if (Pin <= GPIO_PIN_ALL)
    {
        GPIO_InitStruct(&GPIO_InitStructure);
        GPIO_InitStructure.Pin          = Pin;
        GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
        GPIO_InitStructure.GPIO_Pull    = GPIO_No_Pull;
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
        GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);
    }
}
uint8_t KeyMonitor(uint8_t GpioKey)
{
	float Key = 0;
	Key = (float)ADC_GetData(GpioKey)*3.3/1023;
	if(GpioKey == ADC_CH_3_PA2)
	{		
		if((1.4<Key) && (Key<1.6)){
			return Key13;
		}
		else if((2.34<Key) && (Key<2.54)){
			return Key14;
		}
		else if((0.84<Key) && (Key<1.04))
		{
			return Key23; 
		}
		else if((2.05<Key) && (Key<2.25))
		{
		    return Key24;
			}   
		else if((1.75<Key) && (Key<1.95)){
			return Key34;
		}			
		else if((2.68<Key) && (Key<2.9)){
			return Key25;
		}
		else if((2.9<Key) && (Key<3.1)){
			return Key26;
		}
		else
			return 0;
	}
	return 0;
}	
uint8_t KeyHandle()
{
	if(KeyMonitor(ADC_CH_3_PA2)!=0)
	{
		ShakeCnt1++;
		if(ShakeCnt1==1)
		{
			return KeyMonitor(ADC_CH_3_PA2);
		}
	}
	else
	{
		ShakeCnt1 = 0;
	}
	return 0;
}
void key() // 7ms get in
{
	uint8_t KeyValue = 0;
	uint8_t k[3];
	float Key = 0;
	Key = (float)ADC_GetData(ADC_CH_3_PA2)*3.3/1023;
	KeyValue = KeyHandle();
	if(KeyValue!=0)
	{
		if(KeyValue==Key13)
		{
			k[0] = 0x00;
			k[1] = 0x13;
			i2c_write_block(0x02,2,0,k);
			I2C2_GPIO_init();
		}
		if(KeyValue==Key26)
		{
			k[0] = 0x00;
			k[1] = 0x26;
			i2c_write_block(0x02,2,0,k);
			I2C2_GPIO_init();
		}
		if(KeyValue==Key34)
		{
			k[0] = 0x00;
			k[1] = 0x34;
			i2c_write_block(0x02,2,0,k);
			I2C2_GPIO_init();
		}
		if((2.34<Key) && (Key<2.54))
		{
			k[0] = 0x00;
			k[1] = 0x14;
			i2c_write_block(0x02,2,0,k);
			I2C2_GPIO_init();
		}
		if((2.68<Key) && (Key<2.9))
		{
			k[0] = 0x00;
			k[1] = 0x25;
			i2c_write_block(0x02,2,0,k);
			I2C2_GPIO_init();
		}
		if((0.84<Key) && (Key<1.04))
		{
			k[0] = 0x00;
			k[1] = 0x23;
			i2c_write_block(0x02,2,0,k);
			I2C2_GPIO_init();
		}
		if((2.05<Key) && (Key<2.25))
		{
			k[0] = 0x00;
			k[1] = 0x24;
			i2c_write_block(0x02,2,0,k);
			I2C2_GPIO_init();
		}
	}
}

