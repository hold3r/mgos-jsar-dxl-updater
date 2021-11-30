#include "fw/fw_common.h"
#include "fw/fw_ir_rx.h"
#include "fw/fw_imu_9250.h"
#include "fw/fw_color.h"
#include "fw/fw_icm20948.h"

#define MODEL_BALL     (0x02)
#define MODEL_BUT      (0x03)
#define MODEL_DRIVE1   (0x05)
#define MODEL_DRIVE2   (0x06)
#define MODEL_ENCODER  (0x07)
#define MODEL_LED      (0x09)
#define MODEL_POT      (0x13)
#define MODEL_RGB      (0x15)
#define MODEL_MOS      (0x11)
#define MODEL_ZUMMER   (0x18)
#define MODEL_NOISE    (0x12)
#define MODEL_LINE     (0x0A)
#define MODEL_USW      (0x17)
#define MODEL_IR       (0x08)
#define MODEL_COLOR    (0x04)
#define MODEL_FORCE    (0x20)
#define MODEL_IMU9250  (0x10)
#define MODEL_IMU20948 (0x30)

uint8_t firmware_get(uint8_t id, const uint8_t ** fw, uint16_t * size)
{
    switch (id) {
    case MODEL_BALL:
    case MODEL_BUT:
    case MODEL_ENCODER:
    case MODEL_LED:
    case MODEL_POT:
    case MODEL_RGB:
    case MODEL_MOS:
    case MODEL_ZUMMER:
    case MODEL_NOISE:
    case MODEL_LINE:
    case MODEL_USW:
        *fw = fw_common;
        *size = FW_COMMON_SIZE;
        break;

    case MODEL_IR:
        *fw = fw_ir_rx;
        *size = FW_IR_RX_SIZE;
        break;

    case MODEL_COLOR:
        *fw = fw_color;
        *size = FW_COLOR_SIZE;
        break;

    case MODEL_FORCE:
        *fw = NULL;
        *size = 0;
        break;

    case MODEL_IMU9250:
        *fw = fw_imu_9250;
        *size = FW_IMU_9250_SIZE;
        break;

    case MODEL_IMU20948:
        *fw = fw_icm20948;
        *size = FW_ICM20948_SIZE;
        break;

    default:
        *fw = NULL;
        *size = 0;
        break;
    }

    return DXLUP_ERR_NONE;
}