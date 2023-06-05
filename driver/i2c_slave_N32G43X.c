
#include "i2c_slave_N32G43X.h"
#include <stdbool.h>
#include "utils.h"
#include "counter.h"
#include "app_log.h"
#include "app_mcu_bus.h"

uint8_t data_buf[100];
static __IO uint32_t I2CTimeout;
uint8_t flag_slave_send_finish         = 0;
uint8_t flag_slave_recv_finish         = 0;
static uint8_t rxDataNum = 0;
static uint8_t RCC_RESET_Flag = 0;
extern uint8_t dev_addr;
uint8_t Data_prt;
uint8_t read_add;
uint8_t FILTER_PI;
uint16_t R_temp = 0;
uint16_t G_temp = 0;
uint16_t B_temp = 0;
uint16_t BP_temp = 0;
uint16_t DR_temp = 0;
uint16_t motor_step;
uint8_t BP_current;
uint8_t R_FAN_SPEED;
uint8_t G_FAN_SPEED;
uint8_t B_FAN_SPEED;
uint8_t BP_FAN_SPEED;
uint8_t DR_FAN_SPEED;
uint8_t PI_motor_focal;
uint8_t PI_motor_zoom;
uint8_t PI_motor_flag;
uint8_t PI_motor_focal_c;
uint8_t PI_motor_zoom_c;

// Bus Error Event Interrupt Handlers
void (*I2C_0_bus_error_interrupt_handler)(void) = NULL;
// Bus Collision Event Interrupt Handlers
void (*I2C_0_collision_interrupt_handler)(void) = NULL;
// Stop Event Interrupt Handlers
void (*I2C_0_stop_interrupt_handler)(void)= NULL;
// Read Event Interrupt Handlers
void (*I2C_0_read_interrupt_handler)(void) = NULL;
// Write Event Interrupt Handlers
void (*I2C_0_write_interrupt_handler)(void) = NULL;
// Address Event Interrupt Handlersstatic
void (*I2C_0_address_interrupt_handler)(void) = NULL;

extern void read_data_check(void);
extern void msg_dispatch_poll_1(void);

void NVIC_Configuration_I2C(uint8_t ch)
{
    NVIC_InitType NVIC_InitStructure;
    if (ch == 1)
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    }
    if (ch == 2)
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
    }
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    if (ch == 1)
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn; // test err
    }
    if (ch == 2)
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn; // test err
    }
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void I2C_ResetBusy(I2C_Module* I2Cx)
{
    I2Cx->CTRL1 |= 0x8000;  // CTRL1_SWRESET_SET
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    I2Cx->CTRL1 &= ~0x8000;  // CTRL1_SWRESET_RESET
}

void I2C2_GPIO_init(void)
{
	GPIO_InitType i2c2_gpio;

	//RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
	/*PC0 -- SCL; PC1 -- SDA*/
	GPIO_InitStruct(&i2c2_gpio);
	i2c2_gpio.Pin               = GPIO_PIN_10 | GPIO_PIN_9;
	i2c2_gpio.GPIO_Slew_Rate    = GPIO_Slew_Rate_High;
	i2c2_gpio.GPIO_Mode         = GPIO_Mode_AF_OD;
	i2c2_gpio.GPIO_Alternate    = GPIO_AF6_I2C2;
	i2c2_gpio.GPIO_Pull         = GPIO_Pull_Up;	  
	GPIO_InitPeripheral(GPIOA, &i2c2_gpio);
}

/**
 * \brief Initialize I2C interface
 * If module is configured to disabled state, the clock to the I2C is disabled
 * if this is supported by the device's clock system.
 *
 * \return Nothing
 */
void I2C2_init(uint8_t address)
{
    I2C_InitType i2c2_SlaveInit;
	
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_I2C2, ENABLE);
	
    I2C_DeInit(I2C2);     //复位I2C寄存器
    I2C_ResetBusy(I2C2);  //复位BUSY信号

		I2C_DeInit(I2C2);
    i2c2_SlaveInit.BusMode     = I2C_BUSMODE_I2C;
	i2c2_SlaveInit.OwnAddr1    = address;
    i2c2_SlaveInit.FmDutyCycle = I2C_FMDUTYCYCLE_2;
    i2c2_SlaveInit.AckEnable   = I2C_ACKEN;
    i2c2_SlaveInit.AddrMode    = I2C_ADDR_MODE_7BIT;
    i2c2_SlaveInit.ClkSpeed    = 100000; //400K	
    I2C_Init(I2C2, &i2c2_SlaveInit);
//    I2C_ConfigOwnAddr2(I2C1,0x24);
    // int enable
	NVIC_Configuration_I2C((uint8_t)2);
    I2C_ConfigInt(I2C2, I2C_INT_EVENT | I2C_INT_BUF | I2C_INT_ERR, ENABLE);
    
    I2C_Enable(I2C2, ENABLE);
}

/* ################iic 异常侦测 patch############### */
uint32_t iic_time_cnt = 0; //iic起始时间戳
uint8_t iic_start_flag = 0;    //iic开始通信的标志
/* ############################################# */

void I2C2_EV_IRQHandler(void)
{
    uint8_t timeout_flag = 0;
    uint32_t last_event = I2C_GetLastEvent(I2C2);
		
    if ((last_event & I2C_ROLE_MASTER) != I2C_ROLE_MASTER) // MSMODE = 0:I2C slave mode
    {
        switch (last_event)
        {
					/*  */
					case I2C_EVT_SLAVE_SEND_ADDR_MATCHED:  // 0x00060082.EV1.EV3_1 (ADDRF TXDATE)
						rxDataNum=0;	
						read_add = data_buf[0];
						//read_data_check();
						I2C_SendData(I2C2, data_buf[rxDataNum++]);
					break;
					case I2C_EVT_SLAVE_DATA_SENDING:       // 0x00060080.EV3 (TXDATE)
						I2C_SendData(I2C2, data_buf[rxDataNum++]);
					break;
					
					case I2C_EVT_SLAVE_DATA_SENDED:        // 0x00060084.EV3_2 (TXDATE BSF)
						I2C_SendData(I2C2, data_buf[rxDataNum++]);
						//GPIOB->PBC = GPIO_PIN_13;
					break;

					case I2C_EVT_SLAVE_RECV_ADDR_MATCHED:  // 0x00020002.EV1 (ADDRF)
						rxDataNum = 0;
						GPIOB->PBSC = GPIO_PIN_13;
					break;
					
					case I2C_EVT_SLAVE_DATA_RECVD:  // 0x00020040.EV2 (RXDATNE)
							data_buf[rxDataNum++] = I2C_RecvData(I2C2);
					break;
					
					case I2C_EVT_SLAVE_STOP_RECVD:  // 0x00000010.EV4 (STOPF)
						//I2C_0_stop_callback();
					I2C_Enable(I2C2, ENABLE);
						msg_dispatch_poll_1();
						rxDataNum = 0;
//						I2C_Enable(I2C1, ENABLE);
					break;
					
					default:
                        I2C2_init(MCUBUS_I2C_SLAVE_CFG_HARD_ADDR);
						I2C_Enable(I2C2, ENABLE);
           break;
        }
    }
		
}
uint16_t r_current;
uint16_t g_current;
uint16_t b_current;
 
void LedInit_out(GPIO_Module* GPIOx, uint16_t Pin)
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
void I2C2_ER_IRQHandler(void)
{
		
	uint32_t last_event;
    last_event = I2C_GetLastEvent(I2C2);
	
		if ((last_event & I2C_EVT_SLAVE_ACK_MISS) == I2C_EVT_SLAVE_ACK_MISS) {
        I2C_ClrFlag(I2C2, I2C_FLAG_ACKFAIL);  //作为从机缺失主机ACK，清除错误标志，不复位
    } else {
        I2C2_init(MCUBUS_I2C_SLAVE_CFG_HARD_ADDR);
    }
}
void msg_dispatch_poll_1(void)
{
	
		if(data_buf[0] == 0xBB&&data_buf[1] == 0x00)
		{
			R_temp=data_buf[2]+data_buf[3]*256;
			G_temp=data_buf[4]+data_buf[5]*256;
			B_temp=data_buf[6]+data_buf[7]*256;
			BP_temp=data_buf[8]+data_buf[9]*256;
			DR_temp=data_buf[10]+data_buf[11]*256;
		}
		else if(data_buf[0] == 0xBC)
		{
			R_FAN_SPEED = data_buf[1];
			G_FAN_SPEED = data_buf[2];
			B_FAN_SPEED = data_buf[3];
			BP_FAN_SPEED = data_buf[4];
			DR_FAN_SPEED = data_buf[5];
		}
		else if(data_buf[0] == 0x00)
		{
			BP_current = data_buf[1];
			FILTER_PI = 0;
		}
		else if(data_buf[0] == 0x01)
		{
			BP_current = data_buf[1];
			FILTER_PI = 1;
		}
		else if(data_buf[0] == 0x10)
		{
			motor_step = data_buf[1]+data_buf[2]*256;
			r_current = data_buf[3]+data_buf[4]*256;
			g_current = data_buf[5]+data_buf[6]*256;
			b_current = data_buf[7]+data_buf[8]*256;
				
		}
		else if(data_buf[0] == 0xBE)
		{
			PI_motor_zoom = data_buf[1];
			PI_motor_focal = data_buf[2];
			PI_motor_flag = data_buf[3];
		}
		else if(data_buf[0] == 0xBF)
		{
			PI_motor_zoom = data_buf[1];
			PI_motor_focal = data_buf[2];
		}
		else if(data_buf[0] == 0xBD)
		{
			PI_motor_focal_c = data_buf[1];
			PI_motor_zoom_c = data_buf[2];
			PI_motor_flag = data_buf[3];
		}
}