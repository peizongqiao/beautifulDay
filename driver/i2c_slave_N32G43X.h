/**
 * \file
 *
 * \brief I2C slave driver.
 *
 (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */
#ifndef I2C_SLAVE_AT1616_H
#define I2C_SLAVE_AT1616_H

#include <n32g43x_start_pins.h>
#include "utils.h"
#include "GMtypes.h"
#include "bsp_i2c_slave_api_types.h"
#include "app_mcu_bus.h"

#ifdef __cplusplus
extern "C" {
#endif


// ============================ Configure ========================

// ע�⣺������ʹ�õ�Ӳ��I2C Slave�豸
// SDA(PA1)
#define I2C_SLAVE_CFG_SDA_PORT              GPIOC
#define I2C_SLAVE_CFG_SDA_PIN               GPIO_PIN_1
#define I2C_SLAVE_CFG_SDA_PIN_NUM           1
//#define I2C_SLAVE_CFG_SDA_PORTMUX()         do{PORTMUX.CTRLB |= PORTMUX_TWI0_bm;}while(0)

// SCL(PA2)
#define I2C_SLAVE_CFG_SCL_PORT              GPIOC
#define I2C_SLAVE_CFG_SCL_PIN               GPIO_PIN_0
#define I2C_SLAVE_CFG_SCL_PIN_NUM           0
//#define I2C_SLAVE_CFG_SCL_PORTMUX()         do{PORTMUX.CTRLB |= PORTMUX_TWI0_bm;}while(0)

extern void HC595SendData(uint16_t SendVal);

static inline void I2C_SDA_set_pull_off(void)
{
    io_set_pull_off_mode(I2C_SLAVE_CFG_SDA_PORT, I2C_SLAVE_CFG_SDA_PIN_NUM);
}

static inline void I2C_SDA_set_pull_up(void)
{
    io_set_pull_up_mode(I2C_SLAVE_CFG_SDA_PORT, I2C_SLAVE_CFG_SDA_PIN_NUM);
}

static inline void I2C_SDA_set_dir_in(void)
{
    io_set_dir_in(I2C_SLAVE_CFG_SDA_PORT, I2C_SLAVE_CFG_SDA_PIN_NUM);
}


static inline void I2C_SDA_set_dir_out(void)
{
    io_set_dir_out(I2C_SLAVE_CFG_SDA_PORT, I2C_SLAVE_CFG_SDA_PIN_NUM);
}


///*
// * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
// *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
// *                PORT_ISC_RISING_gc        = Sense Rising Edge
// *                PORT_ISC_FALLING_gc       = Sense Falling Edge
// *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
// *                PORT_ISC_LEVEL_gc         = Sense low Level
// */
//static inline void I2C_SDA_set_isc(const PORT_ISC_t isc)
//{
//    io_set_isc(I2C_SLAVE_CFG_SDA_PORT, I2C_SLAVE_CFG_SDA_PIN, isc);
//}


/*
 * is inverted
 */
//static inline void I2C_SDA_set_enable_inverted(void)
//{
//    io_set_enable_inverted(I2C_SLAVE_CFG_SDA_PORT, I2C_SLAVE_CFG_SDA_PIN);
//}
//
//
//// is not inverted
//static inline void I2C_SDA_set_disable_inverted(void)
//{
//    io_set_disable_inverted(I2C_SLAVE_CFG_SDA_PORT, I2C_SLAVE_CFG_SDA_PIN);
//}



/**
 * \brief Set I2C_SDA level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void I2C_SDA_set_level_high(void)
{
    io_set_high(I2C_SLAVE_CFG_SDA_PORT, I2C_SLAVE_CFG_SDA_PIN);
}

static inline void I2C_SDA_set_level_low(void)
{
    io_set_low(I2C_SLAVE_CFG_SDA_PORT, I2C_SLAVE_CFG_SDA_PIN);
}


static inline void I2C_SCL_set_pull_off(void)
{
    io_set_pull_off_mode(I2C_SLAVE_CFG_SCL_PORT, I2C_SLAVE_CFG_SCL_PIN);
}

static inline void I2C_SCL_set_pull_up(void)
{
    io_set_pull_up_mode(I2C_SLAVE_CFG_SCL_PORT, I2C_SLAVE_CFG_SCL_PIN_NUM);
}

static inline void I2C_SCL_set_dir_in(void)
{
    io_set_dir_in(I2C_SLAVE_CFG_SCL_PORT, I2C_SLAVE_CFG_SCL_PIN_NUM);
}


static inline void I2C_SCL_set_dir_out(void)
{
    io_set_dir_out(I2C_SLAVE_CFG_SCL_PORT, I2C_SLAVE_CFG_SCL_PIN_NUM);
}



/*
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
//static inline void I2C_SCL_set_isc(const PORT_ISC_t isc)
//{
//    io_set_isc(I2C_SLAVE_CFG_SCL_PORT, I2C_SLAVE_CFG_SCL_PIN, isc);
//}


/*
 * is inverted
 */
//static inline void I2C_SCL_set_enable_inverted(void)
//{
//    io_set_enable_inverted(I2C_SLAVE_CFG_SCL_PORT, I2C_SLAVE_CFG_SCL_PIN);
//}


//// is not inverted
//static inline void I2C_SCL_set_disable_inverted(void)
//{
//    io_set_disable_inverted(I2C_SLAVE_CFG_SCL_PORT, I2C_SLAVE_CFG_SCL_PIN);
//}


/**
 * \brief Set I2C_SCL level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void I2C_SCL_set_level_high(void)
{
    io_set_high(I2C_SLAVE_CFG_SCL_PORT, I2C_SLAVE_CFG_SCL_PIN);
}

static inline void I2C_SCL_set_level_low(void)
{
    io_set_low(I2C_SLAVE_CFG_SCL_PORT, I2C_SLAVE_CFG_SCL_PIN);
}




// ====================== declare ==========================

// Bus Error Event Interrupt Handlers
extern void (*I2C_0_bus_error_interrupt_handler)(void);
// Bus Collision Event Interrupt Handlers
extern void (*I2C_0_collision_interrupt_handler)(void);
// Stop Event Interrupt Handlers
extern void (*I2C_0_stop_interrupt_handler)(void);
// Read Event Interrupt Handlers
extern void (*I2C_0_read_interrupt_handler)(void);
// Write Event Interrupt Handlers
extern void (*I2C_0_write_interrupt_handler)(void);
// Address Event Interrupt Handlersstatic
extern void (*I2C_0_address_interrupt_handler)(void);

void I2C2_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);

void I2C2_GPIO_init(void);
void I2C2_init(uint8_t address);

// ע�⣺
// 1���˴�ʹ��ԭ��.C�ļ��ж���ĺ�����
// 2����.h�ж����static������Ϊ�˱�������������(inline)���Դﵽ��ʡROM�ռ��Ŀ�ġ�
// 3����ʹ��#define�궨�壬��ʹ�ú�������ʽʱΪ�ˣ�inline���Դﵽ�ڱ���ʱ����в�������Ŀ�ģ����Ӱ�ȫ�ԡ�
/**
 * \brief Open the I2C for communication. Enables the module if disabled.
 *
 * \return Nothing
 */
static void I2C_0_open(void)
{
    I2C_Enable(I2C2, ENABLE);
}

/**
 * \brief Close the I2C for communication. Disables the module if enabled.
 *
 * \return Nothing
 */
static void I2C_0_close(void)
{
    I2C_Enable(I2C2, DISABLE);
}

/**
 * \brief Read one byte from the data register of I2C_0
 *
 * Function will not block if a character is not available, so should
 * only be called when data is available.
 *
 * \return Data read from the I2C_0 module
 */
static uint8_t I2C_0_read(void)
{
    return I2C_RecvData(I2C2);
}

/**
 * \brief Write one byte to the data register of I2C_0
 *
 * Function will not block if data cannot be safely accepted, so should
 * only be called when safe, i.e. in the read callback handler.
 *
 * \param[in] data The character to write to the I2C
 *
 * \return Nothing
 */
static void I2C_0_write(uint8_t data)
{
    I2C_SendData(I2C2, data);
}

/**
 * \brief Send ACK to received address or data. Should
 * only be called when appropriate, i.e. in the callback handlers.
 *
 * \return Nothing
 */
static void I2C_0_send_ack(void)
{
    I2C_ConfigAck(I2C2, ENABLE);
}

/**
 * \brief Send NACK to received address or data. Should
 * only be called when appropriate, i.e. in the callback handlers.
 *
 * \return Nothing
 */
static void I2C_0_send_nack(void)
{
    I2C_ConfigAck(I2C2, DISABLE);
}

/**
 * \brief Goto unaddressed state. Used to reset I2C HW that are aware
 * of bus state to an unaddressed state.
 *
 * \return Nothing
 */
//static void I2C_0_goto_unaddressed(void)
//{
//    // Reset module
//    TWI0.SSTATUS |= (TWI_DIF_bm | TWI_APIF_bm);
//    TWI0.SCTRLB = TWI_SCMD_COMPTRANS_gc;
//}

/**
 * \brief Callback handler for event where master wishes to read a byte from slave.
 *
 * \return Nothing
 */
static void I2C_0_set_read_callback(i2c_slave_callback_t handler)
{
    I2C_0_read_interrupt_handler = handler;
}


/**
 * \brief Callback handler for event where master wishes to write a byte to slave.
 *
 * \return Nothing
 */
static void I2C_0_set_write_callback(i2c_slave_callback_t handler)
{
    I2C_0_write_interrupt_handler = handler;
}

/**
 * \brief Callback handler for event where slave has received its address.
 *
 * \return Nothing
 */
static void I2C_0_set_address_callback(i2c_slave_callback_t handler)
{
    I2C_0_address_interrupt_handler = handler;
}

/**
 * \brief Callback handler for event where slave has received a STOP condition after being addressed.
 *
 * \return Nothing
 */
static void I2C_0_set_stop_callback(i2c_slave_callback_t handler)
{
    I2C_0_stop_interrupt_handler = handler;
}

/**
 * \brief Callback handler for event where slave detects a bus collision.
 *
 * \return Nothing
 */
static void I2C_0_set_collision_callback(i2c_slave_callback_t handler)
{
    I2C_0_collision_interrupt_handler = handler;
}


/**
 * \brief Callback handler for event where slave detects a bus error.
 *
 * \return Nothing
 */
static void I2C_0_set_bus_error_callback(i2c_slave_callback_t handler)
{
    I2C_0_bus_error_interrupt_handler = handler;
}

// Read Event Interrupt Handlers
static void I2C_0_read_callback(void)
{
    if (I2C_0_read_interrupt_handler) {
        I2C_0_read_interrupt_handler();
    }
}

// Write Event Interrupt Handlers
static void I2C_0_write_callback(void)
{
    if (I2C_0_write_interrupt_handler) {
        I2C_0_write_interrupt_handler();
    }
}

// Address Event Interrupt Handlers
static void I2C_0_address_callback(void)
{
    if (I2C_0_address_interrupt_handler) {
        I2C_0_address_interrupt_handler();
    }
}

// Stop Event Interrupt Handlers
static void I2C_0_stop_callback(void)
{
    if (I2C_0_stop_interrupt_handler) {
        I2C_0_stop_interrupt_handler();
    }
}

// Bus Collision Event Interrupt Handlers
static void I2C_0_collision_callback(void)
{
    if (I2C_0_collision_interrupt_handler) {
        I2C_0_collision_interrupt_handler();
    }
}

// Bus Error Event Interrupt Handlers
static void I2C_0_bus_error_callback(void)
{
    if (I2C_0_bus_error_interrupt_handler) {
        I2C_0_bus_error_interrupt_handler();
    }
}



// ======================== ʵ�ֽӿ� =========================
// �ӿڷ�װ
// �˴���.h�ļ��ж���static�ĺ�����ʽ�ķ�װ��Ϊ������(inline)���Դﵽ��ʡROM�ռ�
//

// i2cӲ����ʼ��(�磺io���жϵ����ã�Ӳ�����õ�ַ��)
static void bsp_i2c_slave_api_init(uint8_t address)
{
    I2C2_init(address);
}

// ��i2c�豸���ڵ͹���״̬(�磺�ر��豸��pins IO���ڵ͹���ģʽ��)
static void bsp_i2c_slave_api_low_pwr_init(void)
{
    //__nop__();
}
static void bsp_i2c_slave_api_open(void)
{
    I2C_0_open();
}
static void bsp_i2c_slave_api_close(void)
{
    I2C_0_close();
}
static uint8_t bsp_i2c_slave_api_read(void)
{
    return I2C_0_read();
}
static void bsp_i2c_slave_api_write(uint8_t data)
{
    I2C_0_write(data);
}
static void bsp_i2c_slave_api_send_ack(void)
{
    I2C_0_send_ack();
}
static void bsp_i2c_slave_api_send_nack(void)
{
    I2C_0_send_nack();
}
//static void bsp_i2c_slave_api_goto_unaddressed(void)
//{
//    I2C_0_goto_unaddressed();
//}
static void bsp_i2c_slave_api_set_read_callback(i2c_slave_callback_t handler)
{
    I2C_0_set_read_callback(handler);
}
static void bsp_i2c_slave_api_set_write_callback(i2c_slave_callback_t handler)
{
    I2C_0_set_write_callback(handler);
}
// i2c host�������豸Ӳ����ַ
static void bsp_i2c_slave_api_set_address_callback(i2c_slave_callback_t handler)
{
    I2C_0_set_address_callback(handler);
}
static void bsp_i2c_slave_api_set_stop_callback(i2c_slave_callback_t handler)
{
    I2C_0_set_stop_callback(handler);
}
static void bsp_i2c_slave_api_set_collision_callback(i2c_slave_callback_t handler)
{
    I2C_0_set_collision_callback(handler);
}
static void bsp_i2c_slave_api_set_bus_error_callback(i2c_slave_callback_t handler)
{
    I2C_0_set_bus_error_callback(handler);
}
void msg_dispatch_poll_1(void);

#ifdef __cplusplus
}
#endif

#endif /* I2C_SLAVE_H */
