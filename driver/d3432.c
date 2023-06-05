/*
 * d3434.c
 *
 * Created: 2021.01.22
 * Author: mingjie.gu
 */
#include "dlp_manager.h"
#include "utils.h"
#include "stdlib.h"

#if defined(P23_540p)   //G07N120 


int8_t safe_read(uint8_t sub_addr, uint8_t *param,
uint8_t para_len,volatile uint8_t *data,
uint8_t data_len)
{
	int8_t ret;
	volatile uint8_t status=0;
	volatile uint8_t try_cnt = 5;
	read_dlp(0xd0, 0, 0, &status, 1);
	while (status != 0x81 && try_cnt-->0) {
		__delay_ms(100);
		read_dlp(0xd0, 0, 0, &status, 1);
	}

	ret = read_dlp(sub_addr, param, para_len, data, data_len);
	return ret;
}

int8_t safe_write(uint8_t sub_addr, volatile uint8_t *value, uint8_t len)
{
	int8_t ret;
	volatile uint8_t status=0;
	volatile uint8_t try_cnt = 5;
	read_dlp(0xd0, 0, 0, &status, 1);
	while (status != 0x81 && try_cnt-->0) {
		__delay_ms(100);
		read_dlp(0xd0, 0, 0, &status, 1);
	}
	ret = write_dlp(sub_addr, value, len);
	return ret;
}


void pattern_white()
{
	uint8_t d[2];
	source_pattern();
	d[0] = 0x00;
	d[1] = 0x70;
	safe_write(0x0b,d,2);
}

void pattern_red()
{	
	uint8_t d[2];
	source_pattern();
	d[0] = 0x00;
	d[1] = 0x10;
	safe_write(0x0b,d,2);
}

void pattern_green()
{
	uint8_t d[2];
	source_pattern();
	d[0] = 0x00;
	d[1] = 0x20;
	safe_write(0x0b,d,2);
}

void pattern_blue()
{
	uint8_t d[2];
	source_pattern();
	d[0] = 0x00;
	d[1] = 0x30;
	safe_write(0x0b,d,2);
}

void pattern_black()
{
	uint8_t d[2];
	source_pattern();
	d[0] = 0x00;
	d[1] = 0x00;
	safe_write(0x0b,d,2);
}

void pattern_magenta()
{	
	uint8_t d[2];
	source_pattern();
	d[0] = 0x00;
	d[1] = 0x50;
	safe_write(0x0b,d,2);
}

void pattern_yellow()
{
	uint8_t d[2];
	source_pattern();
	d[0] = 0x00;
	d[1] = 0x60;
	safe_write(0x0b,d,2);
}

void pattern_grids()
{

}

void splash_0()
{
	//uint8_t d[2];
	//source_splash();
	//d[0] = 0x0d;
	//d[1] = 0x00;
	//i2c_write_block(0x36, 2, 0, d);
	//__delay_ms(200);
	//d[0] = 0x35;
	//i2c_write_block(0x36, 1, 0, d);
	
	source_splash();
	uint8_t d[1];
	d[0] = 0x00;
	safe_write(0x0d,d,1);
	__delay_ms(200);
	safe_write(0x35,d,0);

}

void splash_1()
{	
	source_splash();
	uint8_t d[1];
	d[0] = 0x01;
	safe_write(0x0d,d,1);
	__delay_ms(200);
	safe_write(0x35,d,0);

}

void splash_2()
{
	source_splash();
	uint8_t d[1];
	d[0] = 0x02;
	safe_write(0x0d,d,1);
	__delay_ms(200);
	safe_write(0x35,d,0);
}

void splash_3()
{
	source_splash();
	uint8_t d[1];
	d[0] = 0x03;
	safe_write(0x0d,d,1);
	__delay_ms(200);
	safe_write(0x35,d,0);
}

void splash_4()
{
	source_splash();
	uint8_t d[1];
	d[0] = 0x04;
	safe_write(0x0d,d,1);
	__delay_ms(200);
	safe_write(0x35,d,0);
}

void pic_init()
{
	//pic_add(source_external);
	pic_add(pattern_red);
	pic_add(pattern_green);
	pic_add(pattern_blue);
	pic_add(pattern_white);
	pic_add(pattern_black);
	pic_add(splash_0);
	pic_add(splash_1);
	pic_add(splash_2);
	pic_add(splash_3);
	pic_add(splash_4);
}

void set_look(uint8_t index)
{
	uint8_t d[1];
	d[0] = index;	
	safe_write(0x22,d,1);
}

//??????
void set_aprerture(bool onoff)
{

}

//????????,pattern
void set_actuator(bool onoff)
{

}

void actuator_pattern(bool onoff)
{

}

uint8_t actuator_read_gain()
{

}

void set_actuator_gain(bool onoff)
{

}

bool power_off()
{
	set_current(false);
	PORTC_set_pin_level(1, 1);
	PORTB_set_pin_level(2, 0);
	PORTB_set_pin_level(3, 0);    //proj off
	return true;
}

bool power_on()
{
	PORTC_set_pin_level(1, 0);   //bp 灯供电
	PORTB_set_pin_level(2, 1);

	__delay_ms(70);
	PORTB_set_pin_level(3, 1);  //proj on
	__delay_ms(200);
	
	volatile uint8_t status=0;
	volatile uint8_t try_cnt=20;
	while(try_cnt--){
		__delay_ms(100);
		read_dlp(0xd0, 0, 0, &status, 1);
		if(status == 0x81){
			break;
		}
	}
	if(status != 0x81){
		return false;
	}
		
	set_current(false);
	set_led(true);
	return true;	
}

void proj_reset()
{	
	volatile uint8_t status=0;
	volatile uint8_t try_cnt=5;
	while(try_cnt--){
		__delay_ms(100);
		read_dlp(0xd0, 0, 0, &status, 1);
		if(status == 0x81){
			power_off();
			break;
		}
	}
	if((status!=0x81) && (poweron_flag)){
		poweron_flag = false;   //power on 组合键开关
		if (power_on()) {
			//set_actuator(true);
			pattern_white();
			}else{
			power_off();
		}
	}
}

void set_led(bool onoff)
{
	uint8_t d[1];
	if (onoff) {
		d[0] = 0x07;
		safe_write(0x52,d,1);
	} else {
		d[0] = 0x00;
		safe_write(0x52,d,1);
	}
}

void set_current(bool level)
{	
	if(1){
		#if defined(G07N100)
			set_rgbcurrent(100, 100, 100);
		#elif defined(G07N120)
			set_rgbcurrent(100, 100, 100);
		#endif
	}
}

void source_external()
{
	uint8_t d[1];
	d[0] = 0x00;
	safe_write(0x05,d,1);
}

void source_splash()
{	
	uint8_t d[1];
	d[0]=0x02;
	safe_write(0x05,d,1);   //切splash
}

void source_pattern()  //切换到pattern
{
	uint8_t d[1];
	d[0]=0x01;
	safe_write(0x05,d,1);
}

#endif
