#ifndef _APP_MCU_BUS_CFG_DABAI_H_
#define _APP_MCU_BUS_CFG_DABAI_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "GMtypes.h"
#include "n32g43x.h"
#include "n32g43x_start_pins.h"

// ====================== configure ========================
// ���ã�Ӳ��������(MCU_IRQ_Status) (PC2)
#undef MCUBUS_TRIGGER_io_set_pull_off
#define MCUBUS_TRIGGER_io_set_pull_off()        io_set_pull_off_mode(GPIOB, 13)

#undef MCUBUS_TRIGGER_GPIO_Out
#define MCUBUS_TRIGGER_GPIO_Out()         io_set_dir_out(GPIOB, 13)

#undef MCUBUS_TRIGGER_GPIO_IN
#define MCUBUS_TRIGGER_GPIO_IN()          io_set_dir_in(GPIOB, 13)

#undef MCUBUS_TRIGGER_io_set_level_high
#define MCUBUS_TRIGGER_io_set_level_high()      io_set_high(GPIOB, GPIO_PIN_13)

#undef MCUBUS_TRIGGER_io_set_level_low
#define MCUBUS_TRIGGER_io_set_level_low()       io_set_low(GPIOB, GPIO_PIN_13)


/* ����MCU IIC������SOC֮��ͨ�ŵĸ�����
*  1.MCU���н������ݱ仯ʱ��ͨ������ƽ��xxms������֪ͨSOC�ˡ�
*  2.XIAOBAI��̬xxx��ƽ
*  3.���ڴ�GPIO���������ܽ����˸��ã�����Ҫ��ʼ��
*/
#undef MCUBUS_TRIGGER_SOC_ENABLE
#define MCUBUS_TRIGGER_SOC_ENABLE()             do{io_set_dir_out(GPIOB, 13); io_set_low(GPIOB, GPIO_PIN_13);}while(0)


#undef MCUBUS_TRIGGER_SOC_DISABLE
#define MCUBUS_TRIGGER_SOC_DISABLE()            do{io_set_dir_out(GPIOB, 13); io_set_high(GPIOB, GPIO_PIN_13);}while(0)




// ============================= IIC������SOCͨ�ţ�����ʹ������ ============================
//�Ĵ������ã�ʹ�ܼĴ���(1)��  �����üĴ���(0)
#undef CONF_EN_REG_START_MODE
#define CONF_EN_REG_START_MODE                  (1)         //(r)�Ĵ��������mcu��������ͬ����ʼģʽ��ͶӰ������������

#undef CONF_EN_REG_SYNC_SOC_STATUS
#define CONF_EN_REG_SYNC_SOC_STATUS             (1)         //(w)�Ĵ����������MCU��ͬ��״̬��

#undef CONF_EN_REG_MCU_STATUS
#define CONF_EN_REG_MCU_STATUS                  (1)         //(r)�Ĵ������mcu�豸״̬

#undef CONF_EN_REG_FROM_SOC_CTRL_FAN
#define CONF_EN_REG_FROM_SOC_CTRL_FAN           (1)         //(w)�Ĵ������mcu���ƼĴ���

#undef CONF_EN_REG_LAMP_MODE
#define CONF_EN_REG_LAMP_MODE                   (1)         //(w)�Ĵ�������ƹ�ģʽ����

#undef CONF_EN_REG_MCU_EVENT
#define CONF_EN_REG_MCU_EVENT                   (1)         //(r)�Ĵ����������SOC MCU�������¼�

#undef CONF_EN_REG_SOC_FAN_STATUS
#define CONF_EN_REG_SOC_FAN_STATUS              (1)         //(r)�Ĵ������ú꣺MCU���Ƶķ���״̬��Ϣ

#undef CONF_EN_REG_FASTPOWERON
#define CONF_EN_REG_FASTPOWERON                 (1)         //(r)�Ĵ��������ٿ���

#undef CONF_EN_REG_CTRL_UPDATE
#define CONF_EN_REG_CTRL_UPDATE                 (1)         //(w)����MCU��������




// �Ĵ���ͨ����
#undef  MCUBUS_NUMS_MCU_FAN_SPEED_CHANNL
#define MCUBUS_NUMS_MCU_FAN_SPEED_CHANNL        (0)         //����ͨ����

#undef  MCUBUS_NUMS_SOC_FAN_SPEED_CHANNL
#define MCUBUS_NUMS_SOC_FAN_SPEED_CHANNL        (1)         //����ͨ����



#ifdef __cplusplus
}
#endif


#endif
