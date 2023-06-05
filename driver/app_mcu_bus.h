#ifndef _APP_MCU_BUS_H_
#define _APP_MCU_BUS_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "GMtypes.h"
//#include "app_door_ctrl.h"
//#include "app_fan_ctrl.h"
#include "config.h"
#include "app_mcu_bus_cfg.h"
#include "n32g43x.h"


// IIC����Ӳ����д��ַ
#define MCUBUS_DEV_WR_ADDR                  (MCUBUS_I2C_SLAVE_CFG_HARD_ADDR | I2C_WR)      //����д��ַ
#define MCUBUS_DEV_RD_ADDR                  (MCUBUS_I2C_SLAVE_CFG_HARD_ADDR | I2C_RD)      //��������ַ



// ================== ͨ��Э�飺�Ĵ������� =====================
//�Ĵ�����ַ((��r/дw)������SOC�˽ǶȽ�������)
#define MCUBUS_REG_TOUCH_PARA               (0X10)      //(r)�Ĵ������qtouch����
#define MCUBUS_REG_BATTERY_TEMP             (0X11)      //(r)�Ĵ����������¶�
#define MCUBUS_REG_BATTERY_STATUS           (0X12)      //(r)�Ĵ����������豸����״̬
#define MCUBUS_REG_SOC_BATTERY_PERCENT      (0x13)      //(w)�Ĵ����������MCU�����ٷֱ�ֵ
#define MCUBUS_REG_START_MODE               (0x14)      //(r)�Ĵ��������mcu��������ͬ����ʼģʽ��ͶӰ������������
#define MCUBUS_REG_SYNC_SOC_STATUS          (0x15)      //(w)�Ĵ����������MCU��ͬ��״̬��
#define MCUBUS_REG_CHKSUM                   (0x16)      //(r/w)�Ĵ������У���
#define MCUBUS_REG_MCU_STATUS               (0x17)      //(r)�Ĵ������mcu�豸״̬
#define MCUBUS_REG_CTRL_MCU_OLD             (0x18)      //(w)�Ĵ������mcu�ɿ��ƼĴ���(�磺����/���ȣ���MCU����´˼Ĵ��������ã��˼Ĵ������ڿ�������Ŀ�ķ��ȡ����ţ�SOC���ڹ����룬���ʽ��˼Ĵ�������)
#define MCUBUS_REG_MCU_APP_SV               (0x19)      //(r)�Ĵ������mcu APP�汾��
#define MCUBUS_REG_LAMP_MODE                (0x1A)      //(w)�Ĵ�������ƹ�ģʽ����
#define MCUBUS_REG_MCU_EVENT                (0x1B)      //(r)�Ĵ����������SOC MCU�������¼�
#define MCUBUS_REG_VOLTAMETER               (0x1D)      //(r)�Ĵ������������Ϣ
#define MCUBUS_REG_TEMP                     (0x1E)      //(r)�Ĵ����������(��λ:1��)
#define MCUBUS_REG_SOC_FAN                  (0x1F)      //(r)�Ĵ������SOC����ת���ķ���״̬��Ϣ
#define MCUBUS_REG_MCU_FAN                  (0x20)      //(r)�Ĵ������MCU����ת���ķ���״̬��Ϣ

#define MCUBUS_DATE                         (0x21)      //(r)�Ĵ������MCU����ת���ķ���״̬��Ϣ



#define MCUBUS_REG_VOLUME_CTL               (0x24)      //(w)�Ĵ������������С����
#define MCUBUS_REG_TRAVEL_MODE              (0x26)      //(w)�Ĵ����������ģʽ��־
#define MCUBUS_REG_CTRL_MCU_FAN             (0x27)      //(w)�Ĵ������mcu���ƼĴ���(���Ʒ���)
#define MCUBUS_REG_FASTPOWERON              (0x28)      //(r)mcu���ٿ���ģʽ
#define MCUBUS_REG_CTRL_MCU_DOOR            (0X29)      //(w)�Ĵ������mcu���ƼĴ���(���Ŷ���)

#define MCUBUS_REG_MCU_BOOTLOADER_SV        (0x32)      //(r)�Ĵ������MCU bootloader�汾
#define MCUBUS_REG_SOC_EEPROM_SAVE_ADDR_START (0x33)    //(r/w)�Ĵ������SOC�洢EEPROM���ݵĿ�ʼ��ַ��
#define MCUBUS_REG_SOC_EEPROM_SAVE_ADDR_END (MCUBUS_REG_SOC_EEPROM_SAVE_ADDR_START+(64-1)) //�Ĵ������SOC�洢EEPROM���ݵĽ�����ַ��


#define MCUBUS_REG_CTRL_UPDATE              (0xF1)      //(w)����MCU��������

#define MCUBUS_REG_NULL                     (0xFF)      //(soc�ӽ�)�Ĵ������������




// ========================== 0x10 ==========================
//Э���������ݣ�qtouc����
typedef struct
{
    uint16_t reference;
    uint16_t delta;
} protocol_touch_para_t;



// ========================= 0x14 ==========================
// Э���������ͣ�mcu��SOCͬ��״̬��Ϣ
typedef enum {
    E_FLAG_PROJECTION_MODE = 0,                 //ͶӰģʽ
    E_FLAG_SOUND_MODE,                          //����ģʽ
    E_FLAG_UPDATE_MODE,                         //����ģʽ
    E_FLAG_NFC_MODE,                            //NFC����ģʽ
    E_FLAG_NULL,                                //NULL
    E_FLAG_TRAVEL_MODE                          //����ģʽ
} protocol_soc_start_mode_t;



// ========================= 0x15 ==========================
// Э���������ͣ�SOC״̬��Ϣ
typedef enum
{
    E_SOC_STATE_NONE        = 0,                //soc���ᷢ�͸��ź�
    E_SOC_STATE_STANDBY     = 2,                //soc����ǰ���ʹ����ź�
    E_SOC_STATE_BOOT        = 3,                //soc��������
    E_SOC_STATE_PROJECTOR   = 4,                //soc�״ν���������ʱ���͵��ź�
    E_SOC_STATE_SPEAKER     = 5,                //soc�״ν�����ģʽ���͵��ź�
    E_SOC_STATE_SOC_UPDATE  = 6,                //soc��鵽�����ļ�������SOC��������ǰ���͵��ź�
    E_SOC_STATE_WORK        = 7,                //����ģʽ
#if 0
    E_SOC_STATE_BTCONNECT_SUCCESS,
    E_SOC_STATE_BTCONNECT_FAIL,
    E_SOC_STATE_MUSICPAUSE,
    E_SOC_STATE_MUSICPLAY,
#endif
}protocol_soc_status_t;



// ========================= 0x17 ==========================
// Э����������:MCU��״̬�Ĵ���
typedef struct
{
    uint8_t eeprom_is_ready:1;                  // eeprom�Ƿ�׼�������ɶ�д��1-������0-δ������
    uint8_t reserve1:7;                         // ����
}protocol_mcu_status_t;

// =========================== 0x18 ========================
// ���Ŷ���
typedef enum
{
    ENUM_MSTATE_FRONT = 0,
    ENUM_MSTATE_BACK,
    ENUM_MSTATE_STOP,
    ENUM_MSTATE_HWTEST,
    ENUM_MSTATE_ACK,
    ENUM_MSTATE_FRONT_TEST = 5,
    ENUM_MSTATE_BACK_TEST = 6,
    ENUM_MSTATE_NULL,
}protocol_door_action_t;

#pragma pack(1)
typedef struct
{
    uint8_t action;
    uint8_t speed;
}protocol_ctrl_mcu_door_t;
#pragma pack()
// ========================= 0x1E ==========================
// Э����������:�¶�
#pragma pack(1)
typedef struct
{
    int8_t laser;       // ϵͳ�¶�(B��)
    int8_t env;         // �����¶�
    int8_t dmd;         // DMD�¶�
    int8_t cw;          // ɫ���¶�
    int8_t G_led;       // G��
    int8_t R_led;       // R��
    int8_t BP_led;      // BP��
    int8_t board;       // ������
}protocol_temperature_status_t;
#pragma pack()


// ========================= 0x1F ==========================
// Э���������ͣ��������
// ת�ٵ�λ����ת/�֣�r/min��s->m
// ע�⣺��Э������Ҫע����С��
#pragma pack(1)
typedef struct
{
    uint16_t speed;                             // ����ת��(units:r/min)
}protocol_fan_status_t;
#pragma pack()



// ========================== 0x1D =========================
// Э���������ͣ�����������
// ע�⣺��Э������Ҫע����С��
#pragma pack(1)
typedef struct
{
    struct
    {
        uint8_t standbyIsCharged:1;         // standbyģʽ�£��Ƿ����磨bool���ͣ�
        uint8_t reserve1:7;                 // ����
    };
    uint8_t reserve;                        // ����
    uint16_t voltage_mv;                    // ��ص�ѹ��mv������little-endin���䣩
    uint8_t percent;                        // ��ص����ٷֱ�
}protocol_voltameter_t;
#pragma pack()

// ========================== 0x12 =========================
// ������״̬
typedef enum
{
    E_BAT_DEV_STATUS_NORMAL     = 0x55,
    E_BAT_DEV_STATUS_FAULT      = 0x50
}protocol_battery_status_t;




// ========================== 0x1B =========================
// Э���������ͣ�����״̬
typedef enum
{
    E_PROTO_DOOR_STATE_OPEN          = 0xF0, // �򿪵�
    E_PROTO_DOOR_STATE_CLOSE         = 0xE0, // �رյ�
    E_PROTO_DOOR_STATE_CHECK_OK      = 0xD0, // �Լ�ɹ�
    E_PROTO_DOOR_STATE_CHECK_FAIL    = 0xC0  // �Լ�ʧ��
}protocol_door_state_t;



// Э���������ͣ������¼���Ϣ����
typedef enum
{
    E_KEY_MSG_NULL       = 0x00,
    E_KEY_MSG_PLUS       = 0x01,         //SOC �źţ�������
    E_KEY_MSG_MINUS      = 0x02,         //SOC �źţ�������
    E_KEY_MSG_PLAY       = 0x04,         //SOC �źţ�������ͣ
    E_KEY_MSG_NEXT       = 0x08,         //SOC �źţ�ý����һ��
    E_KEY_MSG_POWER      = 0x10,         //SOC �źţ�mcu��Դ
    E_KEY_MSG_NEAR       = 0x0C,         //SOC �źţ��ӽ���Ӧ����
    E_KEY_MSG_LEAVE      = 0x0D,         //SOC �źţ��ӽ���Ӧ�ͷ�
    E_KEY_MSG_PRESS      = 0X12,         //SOC �źţ�pwr���� ATV��Я�����ڹػ�
    E_KEY_MSG_RELASE     = 0X13,         //SOC �źţ�pwr�ͷ�
    E_KEY_MSG_AUDIO      = 0x20,          //SOC �źţ�����ģʽ
}protocol_key_msg_t;


// Э���������ͣ�typeC��״̬��
typedef enum
{
    E_PROTO_ADAPTER_INVALID  = 0x00,        //���ݲ��ɿ�
    E_PROTO_ADAPTER_INSERT   = 0x01,        //����һ˲��
    E_PROTO_ADAPTER_REMOVE   = 0x02,        //�γ�һ˲��
    E_PROTO_ADAPTER_HIGH     = 0x04,        //�ߵ�ƽ״̬��ʱ�����
    E_PROTO_ADAPTER_LOW      = 0x08,        //�͵�ƽ״̬��ʱ�����
}protocol_adapter_status_t;



// Э���������ͣ������¼����ɴӻ�socʵʱ���£�soc��mcu��ȡ
// ע�⣺enumö�����ͣ��������ڱ�������ͬռ���ֽ�������
#pragma pack(1)
typedef struct
{
    struct {
        protocol_key_msg_t key_event;                       // �����¼�
        protocol_door_state_t door_state;                   // �����¼�
    }event;

    protocol_adapter_status_t adapter_plug_status;      // ������״̬
}protocol_mcu_event_t;
#pragma pack()

// =========================== 0x27 ========================
// Э���������ͣ�����mcu�Ĵ���
#pragma pack(1)
typedef struct
{
    protocol_door_action_t unused;           // ��ʷԭ��֮ǰ���ڲ�������ȹ��ÿ��ƼĴ������������ֽڱ����ſ���ռ��
    #pragma pack(1)
    struct
    {
        uint8_t mode;                             // ����ģʽ��1-�������߷���߼���0-MCU���߷���߼���
        uint8_t speed[MCUBUS_NUMS_FAN_SET_CTRL_CHANNL];
    }fan;
    #pragma pack()
}protocol_ctrl_mcu_fan_t;
#pragma pack()



// ========================== 0xF1 =========================
// Э���������ͣ�������ʽ
#pragma pack(1)
typedef struct
{
    uint8_t start;
    uint8_t len;
    uint8_t reserve;
    uint8_t end;
}protocol_update_frame_format_t;
#pragma pack()






// ================== external function ====================
void mcubus_init(void);
void mcubus_i2c_low_pwr_init(void);
void mcubus_i2c_init(void);
void mcubus_i2c_addr_match_ISR_register(void *cb);
void mcubus_check_bus_timeout(void);
void mcubus_trigger_soc(void);
// ���ڽ������
static void mcubus_untrigger_soc(void)
{
    MCUBUS_TRIGGER_SOC_DISABLE();
}


void mcubus_update_regarea_start_mode(protocol_soc_start_mode_t start_mode);
void mcubus_update_regarea_sv(const char *version);
void mcubus_update_regarea_bootloader_sv(void);
//void mcubus_update_regarea_mcu_fan(measure_t *pdata);
//void mcubus_update_regarea_soc_fan(measure_t *pdata);
void mcubus_update_regarea_voltameter_mv(uint16_t mv);
void mcubus_update_regarea_keyevent_motor(void *pdata);
void mcubus_update_regarea_adapter_plug_status(protocol_adapter_status_t status);
//void mcubus_update_regarea_door(door_state_t state);
void mcubus_update_regarea_key_package(uint8_t idx, uint8_t event);
void mcubus_update_regarea_battery_temputer(uint8_t bat_temp);
void mcubus_update_regarea_battery_status(protocol_battery_status_t bat_status);
void mcubus_update_regarea_eeprom_status(BOOL status);
void mcubus_update_regarea_voltameter_percentage(uint8_t voltameter_percentage);
void mcubus_update_regarea_touch_para(void);
void mcubus_update_regarea_temperature(uint8_t *p_temperature);
void mcubus_update_regarea_fast_poweron(BOOL is_fast_poweron);


#ifdef __cplusplus
}
#endif


#endif
