#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include "i2c_master.h"
#include "dlp_manager.h"
#include "bsp_uart.h"
#include "../driver/bsp_adc.h"
#include "../driver/adc_basic.h"
#include "../driver/driver_init.h"
#include "bsp_mult_pwm.h"
#include "../driver/app_log.h"
#include "../driver/clkctrl.h"
#include "app_softtimer.h"
#include "i2c_slave_N32G43X.h"
#include "app_focal_ctrl.h"
#include "bsp_uart.h"
#include "app_softtimer.h"
#include "IR.h"
extern bool flag_900us;
      
volatile int i = 0,TEMP_FLAG_cnt=0;
float v,vol;
static uint16_t c;
extern bool flag_1ms;
extern bool flag_900us;
extern bool flag_20ms;
extern uint8_t flag_count;
uint16_t printf_temp;
uint16_t close_temp;
bool flag_temp = false;
int8_t tepm_r;
int8_t tepm_g;
int8_t tepm_b;
int8_t tepm_bp;
int8_t tepm_dr;


//void print_temp();
void LedInit(GPIO_Module* GPIOx, uint16_t Pin);
void LedOff(GPIO_Module* GPIOx, uint16_t Pin);
void LedOn(GPIO_Module* GPIOx, uint16_t Pin);
//-----------------------------------------------------------------------
//register address(command)
#define REG_TEMP_SET	0x01
#define REG_TEMP_OFFS	0x02
#define REG_P			0x05
#define REG_I			0x06
#define REG_D			0x07

#define OLED_ONOFF			0x08


#define FAN_NUM 4

uint8_t FANPWM,FANPWM_1,FANPWM_2,FANPWM_3,FANPWM_4;
float DMD_Temp;

#define  TOGLE_TESTPIN   VPORTA_IN =1<<6;   //测试PIN
#define  SPI0_CS_L  PORTA_set_pin_le\vel(4, false);
#define  SPI0_CS_H  PORTA_set_pin_level(4, true);

#define  PWR_ON_L  PORTC_set_pin_level(0, false);
#define  PWR_ON_H  PORTC_set_pin_level(0, true);

#define  PROJ_ON_L  PORTC_set_pin_level(1, false);
#define  PROJ_ON_H  PORTC_set_pin_level(1, true);


//long data buffer memory structure
typedef struct _MB_REG_COMM_BUF
{
	uint8_t len;
	uint8_t data[256];
}MB_REG_COMM_BUF_t;

//register topology structure
typedef struct _MCUBUS_REG_AREA
{
	uint8_t dev_status;
	float temp_set[5];	//设置温度
	float temp[5];			//实际温度
	float temp_offs[5];
	float fanspeed[5];	//风速
	uint16_t P;
	uint16_t I;
	uint16_t D;
	uint8_t oled_onoff;
	uint8_t ver;
	volatile int keep_alive;
}MCUBUS_REG_AREA_t;


MCUBUS_REG_AREA_t g_t_reg;

void mcubus_cmd_cb(uint8_t cmdval, MB_REG_COMM_BUF_t *cmd_buf)
{
	g_t_reg.keep_alive = 50;
	if (cmd_buf->len != 0) {
		switch (cmdval) {
		case REG_TEMP_SET:
			if (cmd_buf->len == sizeof(g_t_reg.temp_set)) {
				memcpy(g_t_reg.temp_set, cmd_buf->data, sizeof(g_t_reg.temp_set));
			}
			break;
		case REG_TEMP_OFFS:
			if (cmd_buf->len == sizeof(g_t_reg.temp_offs)) {
				memcpy(g_t_reg.temp_offs, cmd_buf->data, sizeof(g_t_reg.temp_offs));		
			}
			break;
		case REG_P:
			if (cmd_buf->len == sizeof(g_t_reg.P)) {
				memcpy(&g_t_reg.P, cmd_buf->data, sizeof(g_t_reg.P));
			}
			break;
		case REG_I:
			if (cmd_buf->len == sizeof(g_t_reg.I)) {
 				memcpy(&g_t_reg.I, cmd_buf->data, sizeof(g_t_reg.I));
			}
			break;
		case REG_D:
			if (cmd_buf->len == sizeof(g_t_reg.D)) {
				memcpy(&g_t_reg.D, cmd_buf->data, sizeof(g_t_reg.D));
			}
			break;
		case OLED_ONOFF:
			if (cmd_buf->len == sizeof(g_t_reg.oled_onoff)) {
				memcpy(&g_t_reg.oled_onoff, cmd_buf->data, sizeof(g_t_reg.oled_onoff));
			}
			break;
		}
		//config_write((uint8_t *)&g_t_reg, sizeof(g_t_reg));
	}
}


static uint8_t PIDCalc( float Sv, float Pv)  // Sv设定温度值  Pv当前温度值
{
  float DERR1 = 0;       //
  float DERR2 = 0;       //

  float Pout = 0;       //比例结果
  float Iout = 0;       //积分结果
  float Dout = 0;       //微分结果
  int16_t Out = 0; //总输出
  static uint8_t Out1;  //记录上次输出

  static float ERR;       //当前误差
  static float ERR1;      //上次误差
  static float ERR2;      //上上次误差

  /******************
  *以下四项是需要根据实际情况调试的
  ******************/
  static uint8_t pidt = 100;     //300MS计算一次 计算周期
  char Kp = g_t_reg.P;       //比例系数
  uint16_t Ti= g_t_reg.I; //积分时间
  uint16_t Td= g_t_reg.D; //微分时间

  static unsigned char Upper_Limit= 100; //PID输出上限
  static unsigned char Lower_Limit= 0; //PID输出下限

  ERR = Pv - Sv;   //算出当前误差
  DERR1 = ERR - ERR1;   //上次
  DERR2 = ERR - 2 * ERR1 + ERR2; //上上次

  Pout = Kp * DERR1;    //输出P
  Iout = (float)(ERR * ((float)(Kp * pidt) / Ti));  //输出I
  Dout = (float)(DERR2 * ((float)(Kp * Td) / pidt));   //输出D
//  Out = (unsigned int)(Out1 + Pout + Iout + Dout);
//  cout << "Pout: " << Pout << ", Iout: " << Iout << endl;
  Out = (Out1 + Pout + Iout);

  if(Out >= Upper_Limit)   //如果输出大于等于上限
  {
    Out = Upper_Limit;
  }
  else if(Out <= Lower_Limit)  //如果输出小于等于下线
  {
    Out = Lower_Limit;
  }
  
  if (Pv < -50) {
	  Out = 100;
  }
  Out1 = Out;      //记录这次输出的值

  ERR2 = ERR1;    //记录误差
  ERR1 = ERR;     //记录误差
  
  return Out1;
}

static uint8_t PIDCalc1( float Sv, float Pv)  // Sv设定温度值  Pv当前温度值
{
  float DERR1 = 0;       //
  float DERR2 = 0;       //

  float Pout = 0;       //比例结果
  float Iout = 0;       //积分结果
  float Dout = 0;       //微分结果
  int16_t Out = 0; //总输出
  static uint8_t Out1;  //记录上次输出

  static float ERR;       //当前误差
  static float ERR1;      //上次误差
  static float ERR2;      //上上次误差

  /******************
  *以下四项是需要根据实际情况调试的
  ******************/
  static uint8_t pidt = 100;     //300MS计算一次 计算周期
  char Kp = g_t_reg.P;       //比例系数
  uint16_t Ti= g_t_reg.I; //积分时间
  uint16_t Td= g_t_reg.D; //微分时间

  static unsigned char Upper_Limit= 100; //PID输出上限
  static unsigned char Lower_Limit= 0; //PID输出下限

  ERR = Pv - Sv;   //算出当前误差
  DERR1 = ERR - ERR1;   //上次
  DERR2 = ERR - 2 * ERR1 + ERR2; //上上次

  Pout = Kp * DERR1;    //输出P
  Iout = (float)(ERR * ((float)(Kp * pidt) / Ti));  //输出I
  Dout = (float)(DERR2 * ((float)(Kp * Td) / pidt));   //输出D
//  Out = (unsigned int)(Out1 + Pout + Iout + Dout);
//  cout << "Pout: " << Pout << ", Iout: " << Iout << endl;
  Out = (Out1 + Pout + Iout);

  if(Out >= Upper_Limit)   //如果输出大于等于上限
  {
    Out = Upper_Limit;
  }
  else if(Out <= Lower_Limit)  //如果输出小于等于下线
  {
    Out = Lower_Limit;
  }
  
  if (Pv < -50) {
	  Out = 100;
  }
  Out1 = Out;      //记录这次输出的值

  ERR2 = ERR1;    //记录误差
  ERR1 = ERR;     //记录误差
  

  return Out1;
}

static uint8_t PIDCalc2( float Sv, float Pv)  // Sv设定温度值  Pv当前温度值
{
  float DERR1 = 0;       //
  float DERR2 = 0;       //

  float Pout = 0;       //比例结果
  float Iout = 0;       //积分结果
  float Dout = 0;       //微分结果
  int16_t Out = 0; //总输出
  static uint8_t Out1;  //记录上次输出

  static float ERR;       //当前误差
  static float ERR1;      //上次误差
  static float ERR2;      //上上次误差

  /******************
  *以下四项是需要根据实际情况调试的
  ******************/
  static uint8_t pidt = 100;     //300MS计算一次 计算周期
  char Kp = g_t_reg.P;       //比例系数
  uint16_t Ti= g_t_reg.I; //积分时间
  uint16_t Td= g_t_reg.D; //微分时间

  static unsigned char Upper_Limit= 100; //PID输出上限
  static unsigned char Lower_Limit= 0; //PID输出下限

  ERR = Pv - Sv;   //算出当前误差
  DERR1 = ERR - ERR1;   //上次
  DERR2 = ERR - 2 * ERR1 + ERR2; //上上次

  Pout = Kp * DERR1;    //输出P
  Iout = (float)(ERR * ((float)(Kp * pidt) / Ti));  //输出I
  Dout = (float)(DERR2 * ((float)(Kp * Td) / pidt));   //输出D
//  Out = (unsigned int)(Out1 + Pout + Iout + Dout);
//  cout << "Pout: " << Pout << ", Iout: " << Iout << endl;
  Out = (Out1 + Pout + Iout);

  if(Out >= Upper_Limit)   //如果输出大于等于上限
  {
    Out = Upper_Limit;
  }
  else if(Out <= Lower_Limit)  //如果输出小于等于下线
  {
    Out = Lower_Limit;
  }
  
  if (Pv < -50) {
	  Out = 100;
  }
  Out1 = Out;      //记录这次输出的值

  ERR2 = ERR1;    //记录误差
  ERR1 = ERR;     //记录误差
  
//    log_info("%f\t",Sv);
//    log_info("%f\t",Pv);
//    log_info("%f\n",Out1);
  return Out1;
}

static uint8_t PIDCalc3( float Sv, float Pv)  // Sv设定温度值  Pv当前温度值
{
  float DERR1 = 0;       //
  float DERR2 = 0;       //

  float Pout = 0;       //比例结果
  float Iout = 0;       //积分结果
  float Dout = 0;       //微分结果
  int16_t Out = 0; //总输出
  static uint8_t Out1;  //记录上次输出

  static float ERR;       //当前误差
  static float ERR1;      //上次误差
  static float ERR2;      //上上次误差

  /******************
  *以下四项是需要根据实际情况调试的
  ******************/
  static uint8_t pidt = 100;     //300MS计算一次 计算周期
  char Kp = g_t_reg.P;       //比例系数
  uint16_t Ti= g_t_reg.I; //积分时间
  uint16_t Td= g_t_reg.D; //微分时间

  static unsigned char Upper_Limit= 100; //PID输出上限
  static unsigned char Lower_Limit= 0; //PID输出下限

  ERR = Pv - Sv;   //算出当前误差
  DERR1 = ERR - ERR1;   //上次
  DERR2 = ERR - 2 * ERR1 + ERR2; //上上次

  Pout = Kp * DERR1;    //输出P
  Iout = (float)(ERR * ((float)(Kp * pidt) / Ti));  //输出I
  Dout = (float)(DERR2 * ((float)(Kp * Td) / pidt));   //输出D
//  Out = (unsigned int)(Out1 + Pout + Iout + Dout);
//  cout << "Pout: " << Pout << ", Iout: " << Iout << endl;
  Out = (Out1 + Pout + Iout);

  if(Out >= Upper_Limit)   //如果输出大于等于上限
  {
    Out = Upper_Limit;
  }
  else if(Out <= Lower_Limit)  //如果输出小于等于下线
  {
    Out = Lower_Limit;
  }
  
  if (Pv < -50) {
	  Out = 100;
  }
  Out1 = Out;      //记录这次输出的值

  ERR2 = ERR1;    //记录误差
  ERR1 = ERR;     //记录误差
  

  return Out1;
}

static uint8_t PIDCalc4( float Sv, float Pv)  // Sv设定温度值  Pv当前温度值
{
  float DERR1 = 0;       //
  float DERR2 = 0;       //

  float Pout = 0;       //比例结果
  float Iout = 0;       //积分结果
  float Dout = 0;       //微分结果
  int16_t Out = 0; //总输出
  static uint8_t Out1;  //记录上次输出

  static float ERR;       //当前误差
  static float ERR1;      //上次误差
  static float ERR2;      //上上次误差

  /******************
  *以下四项是需要根据实际情况调试的
  ******************/
  static uint8_t pidt = 100;     //300MS计算一次 计算周期
  char Kp = g_t_reg.P;       //比例系数
  uint16_t Ti= g_t_reg.I; //积分时间
  uint16_t Td= g_t_reg.D; //微分时间

  static unsigned char Upper_Limit= 100; //PID输出上限
  static unsigned char Lower_Limit= 0; //PID输出下限

  ERR = Pv - Sv;   //算出当前误差
  DERR1 = ERR - ERR1;   //上次
  DERR2 = ERR - 2 * ERR1 + ERR2; //上上次

  Pout = Kp * DERR1;    //输出P
  Iout = (float)(ERR * ((float)(Kp * pidt) / Ti));  //输出I
  Dout = (float)(DERR2 * ((float)(Kp * Td) / pidt));   //输出D
//  Out = (unsigned int)(Out1 + Pout + Iout + Dout);
//  cout << "Pout: " << Pout << ", Iout: " << Iout << endl;
  Out = (Out1 + Pout + Iout);

  if(Out >= Upper_Limit)   //如果输出大于等于上限
  {
    Out = Upper_Limit;
  }
  else if(Out <= Lower_Limit)  //如果输出小于等于下线
  {
    Out = Lower_Limit;
  }
  
  if (Pv < -50) {
	  Out = 100;
  }
  Out1 = Out;      //记录这次输出的值

  ERR2 = ERR1;    //记录误差
  ERR1 = ERR;     //记录误差
  

  return Out1;
}


//-----------------------------------------------------

#define  R0                         100000   //100K
#define  B                          3975
#define  R2T(r)                     (B/(logf(r) - logf(R0) + B/(25+273.15)) - 273.15)

uint16_t TEMP_DMD;
struct tmp_row_s {
	int t_infl;
	float t_c;
	float v_offs;
};

struct tmp_row_s tmp_row[3] = {
	{ 125, 10.6, 1752.5 },
	{ 100, 10.1, 1500 },
	{ 0, 10, 500 },
};

float v2t(float v)
{
	struct tmp_row_s *t;
	
	for (int i=0; i<(sizeof(tmp_row)/sizeof(tmp_row[0])); i++) {
		if (v > tmp_row[i].v_offs) {
			t = &tmp_row[i];
			break;
		}
	}
	return ((v-t->v_offs)/t->t_c+t->t_infl);
}

//type: 0代表电阻，非0代表电压
float ad2temp(uint8_t channal, unsigned char type)
{
	float v;
	float r;
	v = (float)ADC_GetData(channal)*3.3/1024;//    ADC_CH_8_PA7
	r = v*30*1000/(3.3-v);
	if (type == 0) {
		return R2T(r);
		} else {
		return v2t(v*1000);
	}
}

uint16_t TEMP[6] = {0};

int8_t tepm_DMD;

void DMD_TEMP (void)
{
    uint8_t Tepm_DMD;
	tepm_DMD = Flash_Read(0x080179FF);
	DMD_Temp = (ad2temp(ADC_CH_8_PA7, 0)+tepm_DMD);    //G
    TEMP_DMD = DMD_Temp*10;
}	 	
/**
 * @brief  Inserts a delay time.
 * @param count specifies the delay time length.
 */


 
void Delay(uint32_t count)
{
    for (; count > 0; count--)
        ;
}

/**
 * @brief  Configures key GPIO.
 * @param GPIOx x can be A to G to select the GPIO port.
 * @param Pin This parameter can be GPIO_PIN_0~GPIO_PIN_15.
 */
void KeyInputInit(GPIO_Module* GPIOx, uint16_t Pin)
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

    /* Configure the GPIO pin as input floating*/
    if (Pin <= GPIO_PIN_ALL)
    {
        GPIO_InitStruct(&GPIO_InitStructure);
        GPIO_InitStructure.Pin        = Pin;
        GPIO_InitStructure.GPIO_Pull  = GPIO_No_Pull;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Input;
        GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);
    }
}

void LedInit_in(GPIO_Module* GPIOx, uint16_t Pin)
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
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Input;
        GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);
    }
}
/**
 * @brief  Configures LED GPIO.
 * @param GPIOx x can be A to G to select the GPIO port.
 * @param Pin This parameter can be GPIO_PIN_0~GPIO_PIN_15.
 */
void LedInit(GPIO_Module* GPIOx, uint16_t Pin)
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


/**
 * @brief  Turns selected Led on as output low level.
 * @param GPIOx x can be A to G to select the GPIO port.
 * @param Pin This parameter can be GPIO_PIN_0~GPIO_PIN_15.
 */
void LedOn(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIOx->PBSC = Pin;
}
/**
 * @brief  Turns selected Led Off as output high level.
 * @param GPIOx x can be A to G to select the GPIO port.
 * @param Pin This parameter can be GPIO_PIN_0~GPIO_PIN_15.
 */
void LedOff(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIOx->PBC = Pin;
}

void LEDBlink(GPIO_Module* GPIOx,uint16_t Pin)
{
    GPIOx->POD ^= Pin;
}

/**
 * @brief Assert failed function by user.
 * @param file The name of the call that failed.
 * @param line The source line number of the call that failed.
 */
#ifdef USE_FULL_ASSERT
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    while (1)
    {
    }
}
#endif // USE_FULL_ASSERT

/**
 * @brief  Main program.
 */

void HC595SendData(uint16_t SendVal)

{
  uint8_t i;
  LedInit(GPIOB, GPIO_PIN_13);
  LedInit(GPIOA, GPIO_PIN_12);
  LedInit(GPIOC, GPIO_PIN_10);

for (i=0; i<16; i++)

  {


    if (((SendVal << i) & 0x8000) != 0 ) 

	LedOn(GPIOB,GPIO_PIN_13);

	else 

	LedOff(GPIOB,GPIO_PIN_13);


    LedOff(GPIOA,GPIO_PIN_12);

    Delay(10000);

    LedOn(GPIOA,GPIO_PIN_12);

  }
   LedOff(GPIOC,GPIO_PIN_10);
   Delay(10000);
   LedOn(GPIOC,GPIO_PIN_10);
}

void HC595SendData_1(uint16_t SendVal)

{
  uint8_t i;
  LedInit(GPIOA, GPIO_PIN_8);//SER2
  LedInit(GPIOA, GPIO_PIN_15);//SRCLK2
  LedInit(GPIOA, GPIO_PIN_11);//RCLK2

for (i=0; i<16; i++)

  {

    if (((SendVal << i) & 0x8000) != 0 ) 

	LedOn(GPIOA,GPIO_PIN_8);

	else 

	LedOff(GPIOA,GPIO_PIN_8);

    LedOff(GPIOA,GPIO_PIN_15);

    Delay(10);

    LedOn(GPIOA,GPIO_PIN_15);

  }
   LedOff(GPIOA,GPIO_PIN_11);
   Delay(10);
   LedOn(GPIOA,GPIO_PIN_11);
}
extern bool TSP_mode;
extern bool TSP_mode_1;
void TSP_CONTREL()
{
	if(TSP_mode_1==1)
	{
		if(TSP_mode==1)
			HC595SendData_1(0x02);
		else if(TSP_mode==0)
			HC595SendData_1(0x00);
	}
}

//=======================================================================
/** 		用户flash写入函数
	说明：	任意起始地址, 没有对齐限制
			任意数据长度, 支持flash跨页
*/

#define	FLASH_PAGE_SIZE		2048		// flash一页空间为2048字节
static FLASH_STS FLASH_ProgramPage(uint32_t* SrcAddr, uint32_t DstAddr, uint16_t Len)
{
    uint32_t i;

    for (i = 0; i < Len; i += 4)
    {
        FLASH_ProgramWord(DstAddr + i, *(uint32_t*)((uint32_t)SrcAddr + i));
		#if (0)		// 是否开启擦除校验
        if (*(uint32_t*)(DstAddr + i) != *(uint32_t*)((uint32_t)SrcAddr + i))
        {
            return FLASH_ERROR_PG;	
        }
		#endif
    }
    return FLASH_COMPL;
}



FLASH_STS FLASH_Program_User(uint32_t addr, 		// 起始地址
								uint8_t *src, 		// 写入的数据指针
								uint32_t len)		// 数据字节长度		
{
	uint8_t FLASH_PAGE_BUF[FLASH_PAGE_SIZE];
    uint32_t i, flag = 0;
    uint32_t pageAddr, pageCount;
    uint32_t tmpAddr, tmpLen;   
    uint32_t startAddr, endAddr;  

    FLASH_Unlock();
    startAddr = addr % FLASH_PAGE_SIZE;				
    endAddr   = (addr + len) % FLASH_PAGE_SIZE;		
	
    if (startAddr == 0)
    {
        pageAddr = addr;		
        pageCount = len / FLASH_PAGE_SIZE;
        for (i = 0; i < pageCount; i++)
        {
            if (FLASH_COMPL != FLASH_EraseOnePage(pageAddr) != 0)
			{FLASH_Lock();	return FLASH_ERROR_WRP;}
				
            if (FLASH_COMPL != FLASH_ProgramPage((uint32_t*)(src + (i * FLASH_PAGE_SIZE)), pageAddr, FLASH_PAGE_SIZE))
			{FLASH_Lock();	return FLASH_ERROR_WRP;}
            
			pageAddr += FLASH_PAGE_SIZE;
        }
		
        if (endAddr != 0)
        {
            for (i = 0; i < FLASH_PAGE_SIZE; i++)
			{FLASH_PAGE_BUF[i] = ((uint8_t*)pageAddr)[i];}
			
            tmpAddr = len % FLASH_PAGE_SIZE;
            for (i = 0; i < tmpAddr; i++)
            {FLASH_PAGE_BUF[i] = ((uint8_t*)(src + (pageCount * FLASH_PAGE_SIZE)))[i];}
			
            if (FLASH_COMPL != FLASH_EraseOnePage(pageAddr))
            {FLASH_Lock();	return FLASH_ERROR_WRP;}

            if (FLASH_COMPL != FLASH_ProgramPage((uint32_t*)FLASH_PAGE_BUF, pageAddr, FLASH_PAGE_SIZE))
            {FLASH_Lock();	return FLASH_ERROR_WRP;}
        }
    }
    else
    {
        pageAddr = (addr / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;	
        tmpAddr = addr % FLASH_PAGE_SIZE;						
        tmpLen = FLASH_PAGE_SIZE - tmpAddr;						
        
		if (tmpLen > len)	{ tmpLen = len; flag = 0;	}        else				{ flag = 1;					}

        for (i = 0; i < FLASH_PAGE_SIZE; i++)
        {FLASH_PAGE_BUF[i] = ((uint8_t*)pageAddr)[i]; }

        for (i = 0; i < tmpLen; i++)
        { FLASH_PAGE_BUF[tmpAddr + i] = ((uint8_t*)src)[i]; }
		
        if (FLASH_COMPL != FLASH_EraseOnePage(pageAddr))
        {FLASH_Lock();	return FLASH_ERROR_WRP; }
		
        if (FLASH_COMPL != FLASH_ProgramPage((uint32_t*)FLASH_PAGE_BUF, pageAddr, FLASH_PAGE_SIZE))
        {FLASH_Lock(); 	return FLASH_ERROR_WRP; }
		
        pageCount = (len - tmpLen) / FLASH_PAGE_SIZE;
        pageAddr += FLASH_PAGE_SIZE;
        for (i = 0; i < pageCount; i++)
        {
            if (FLASH_COMPL != FLASH_EraseOnePage(pageAddr))
            { FLASH_Lock();	return FLASH_ERROR_WRP; }

            if (FLASH_COMPL != FLASH_ProgramPage((uint32_t*)(src + tmpLen + (i * FLASH_PAGE_SIZE)), pageAddr, FLASH_PAGE_SIZE))
            {FLASH_Lock();	return FLASH_ERROR_WRP; }
			
            pageAddr += FLASH_PAGE_SIZE;
        }
		
        if ((endAddr != 0) && (flag == 1))
        {
            for (i = 0; i < FLASH_PAGE_SIZE; i++)
            {FLASH_PAGE_BUF[i] = ((uint8_t*)pageAddr)[i]; }
			
            tmpAddr = (len - tmpLen) % FLASH_PAGE_SIZE;
			
            for (i = 0; i < tmpAddr; i++)
            {FLASH_PAGE_BUF[i] = ((uint8_t*)(src + tmpLen + (pageCount * FLASH_PAGE_SIZE)))[i]; }

            if (FLASH_COMPL != FLASH_EraseOnePage(pageAddr))
            {FLASH_Lock();	return FLASH_ERROR_WRP; }

            if (FLASH_COMPL != FLASH_ProgramPage((uint32_t*)FLASH_PAGE_BUF, pageAddr, FLASH_PAGE_SIZE))
            {FLASH_Lock();	return FLASH_ERROR_WRP; }
        }
    }

    FLASH_Lock();
    return FLASH_COMPL;
}


uint32_t Flash_Read(uint32_t address)
{
    uint32_t retval;
    retval = *(uint16_t *)(address);
    return retval;
}



uint8_t b[4];
uint8_t count;
extern uint8_t log_number,log_flag;
extern uint8_t RxBuffer[5];
extern bool flag_700us;
int main(void)
{
	CLKCTRL_init();
	
	bsp_adc_init();
	/*SystemInit() function has been called by startup file startup_n32g43x.s*/
	LedInit(GPIOC, GPIO_PIN_13);//GPIOC_13   PWR_ON/OFF
	LedInit(GPIOC, GPIO_PIN_11);//GPIOC_11   proj_on
	LedInit(GPIOA, GPIO_PIN_15);
	LedInit(GPIOC, GPIO_PIN_3);
	LedInit(GPIOB, GPIO_PIN_14);

	//    /* Initialize Led1~Led2 as output pushpull mode*/
	LedOn(GPIOC,GPIO_PIN_13);//MCU0关闭PC13和PC11
	LedOn(GPIOC,GPIO_PIN_11);	
	LedOff(GPIOC,GPIO_PIN_3);	

	i2c_init();
	__delay_ms(10);
	I2C2_GPIO_init();
	bsp_i2c_slave_api_init(MCUBUS_I2C_SLAVE_CFG_HARD_ADDR);//0x80
	bsp_i2c_slave_api_open();
	
	TASK_START();

	
while (1)
  {
	  task();  
  }
}



/**
 * @
 */
