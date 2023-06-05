/*
* ģ������ : mcu_bus IIC����ͨ�� �������ã�Ĭ�ϣ�
* �ļ����� : xxxx.h
* ˵     �� :
*/
#ifndef _APP_MCU_BUS_CFG_H_
#define _APP_MCU_BUS_CFG_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "GMtypes.h"
#include "config.h"



// ============== configure(���ļ��ڽ�ֹ�޸�) ==============
// ���ã�I2C slave�豸Ӳ����ַ
#define MCUBUS_I2C_SLAVE_CFG_HARD_ADDR          0x80u


// ���ã�Ӳ��������(MCU_IRQ_Status)
#define MCUBUS_TRIGGER_io_set_pull_off()        io_set_pull_off_mode(GPIOB, 13)
#define MCUBUS_TRIGGER_GPIO_Out()         io_set_dir_out(GPIOB, 13)
#define MCUBUS_TRIGGER_GPIO_IN()          io_set_dir_in(GPIOB, 13)
#define MCUBUS_TRIGGER_io_set_level_high()      io_set_high(GPIOB, GPIO_PIN_13)
#define MCUBUS_TRIGGER_io_set_level_low()       io_set_low(GPIOB, GPIO_PIN_13)


// ����MCU IIC������SOC֮��ͨ�ŵĸ�����
// 2.MCU���н������ݱ仯ʱ��ͨ������ƽ��xxms������֪ͨSOC�ˡ�
#define MCUBUS_TRIGGER_SOC_ENABLE()             io_set_high(GPIOB, GPIO_PIN_13)
#define MCUBUS_TRIGGER_SOC_DISABLE()            io_set_low(GPIOB, GPIO_PIN_13)



// ========== �Ĵ�������ʹ��(���ļ��ڽ�ֹ�޸�) =============

//�Ĵ�����ַ��r/дw������SOC�˽ǶȽ���������
//�Ĵ������ã�ʹ�ܼĴ���(1)��  ��ʹ�ܼĴ���(0)
#define CONF_EN_REG_TOUCH_PARAM                 (1)         //(r)�Ĵ������qtouch����
#define CONF_EN_REG_BATTERY_TEMP                (0)         //(r)�Ĵ����������¶�
#define CONF_EN_REG_BATTERY_STATUS              (0)         //(r)�Ĵ����������豸����״̬
#define CONF_EN_REG_SOC_BATTERY_PERCENT         (0)         //(w)�Ĵ����������MCU�����ٷֱ�ֵ
#define CONF_EN_REG_START_MODE                  (0)         //(r)�Ĵ��������mcu��������ͬ����ʼģʽ��ͶӰ������������
#define CONF_EN_REG_SYNC_SOC_STATUS             (0)         //(w)�Ĵ����������MCU��ͬ��״̬��
#define CONF_EN_REG_CHKSUM                      (1)         //(r/w)�Ĵ������У���
#define CONF_EN_REG_MCU_STATUS                  (0)         //(r)�Ĵ������mcu�豸״̬
#define CONF_EN_REG_FROM_SOC_CTRL_DOOR          (0)         //(w)�Ĵ������mcu���ƼĴ���������ͬһ�Ĵ�����
#define CONF_EN_REG_FROM_SOC_CTRL_FAN           (0)         //(w)�Ĵ������mcu���ƼĴ���������ͬһ�Ĵ�����
#define CONF_EN_REG_MCU_APP_SV                  (1)         //(r)�Ĵ������mcu APP�汾��
#define CONF_EN_REG_LAMP_MODE                   (0)         //(w)�Ĵ�������ƹ�ģʽ����
#define CONF_EN_REG_MCU_EVENT                   (0)         //(r)�Ĵ����������SOC MCU�������¼�
#define CONF_EN_REG_VOLTAMETER                  (0)         //(r)�Ĵ������������Ϣ
#define CONF_EN_REG_TEMP                        (1)         //(r)�Ĵ������ú꣺�¶���Ϣ
#define CONF_EN_REG_SOC_FAN_STATUS              (0)         //(r)�Ĵ������ú꣺SOC���Ƶķ�����Ϣ
#define CONF_EN_REG_MCU_FAN_STATUS              (0)         //(r)�Ĵ������ú꣺MCU���Ƶķ�����Ϣ

#define CONF_EN_REG_TRAVEL_MODE                 (0)         //(w)�Ĵ����������ģʽ��־
#define CONF_EN_REG_FASTPOWERON                 (0)         //(r)�Ĵ��������ٿ���
#define CONF_EN_REG_MCU_BOOTLOADER_SV           (0)         //(r)�Ĵ������MCU bootloader�汾

#define CONF_EN_REG_CTRL_UPDATE                 (1)         //(w)����MCU��������
#define CONF_EN_REG_SOC_EEPROM_SAVE             (0)         //(r/w)�Ĵ������S�Ƿ����ܣ�SOC����EEPROM��MCU�洢��

#define CONF_EN_REG_VOLUME_CTL                  (0)         //(w)�Ĵ������������С����



// ========= �Ĵ������ݴ�С����(���ļ��ڽ�ֹ�޸�) ==========
#define MCUBUS_SIZES_MCU_APP_SV                 (8)         //MCU APP����汾�ֽڳ���

#define MCUBUS_NUMS_TOUCH_CHANNEL               (3)         //qtouch������ͨ����

#define MCUBUS_NUMS_MCU_FAN_SPEED_CHANNL        (1)         //MCU���Ƶķ���״̬ͨ����
#define MCUBUS_NUMS_SOC_FAN_SPEED_CHANNL        (1)         //SOC���Ƶķ���״̬ͨ����

#define MCUBUS_NUMS_FAN_SET_CTRL_CHANNL         (5)         //�������ÿ���ͨ����

#define MCUBUS_SIZES_CHECKSUM                   (2)         //checksum�ֽ���

#define MCUBUS_SIZES_SOC_EEPROM_CACHE           (16)        //SOC EEPROM�洢cache�ֽڴ�С




// ================== ������ͬ��Ŀ���޸� ===================

#include "app_mcu_bus_cfg_dabai.h"

#ifdef __cplusplus
}
#endif


#endif