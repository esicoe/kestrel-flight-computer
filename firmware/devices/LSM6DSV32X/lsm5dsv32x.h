#ifndef LSM5DSV32X_H
#define LSM5DSV32X_H

#include "config.h"
#include "pins.h"

#include "i2c0.h"
#include "led.h"

#define ADDRESS_IMU_I2C                 0x6AU   
#define REGISTER_IMU_FUNC_CFG_ACCESS    0x01U   // R/W
#define REGISTER_IMU_PIN_CTRL           0x02U   // R/W
#define REGISTER_IMU_IF_CFG             0x03U   // R/W
#define REGISTER_IMU_FIFO_CTRL1         0x07U   // R/W
#define REGISTER_IMU_FIFO_CTRL2         0x08U   // R/W
#define REGISTER_IMU_FIFO_CTRL3         0x09U   // R/W
#define REGISTER_IMU_FIFO_CTRL4         0x0AU   // R/W
#define REGISTER_IMU_COUNTER_BDR_REG1   0x0BU   // R/W
#define REGISTER_IMU_INT1_CTRL          0x0DU   // R/W
#define REGISTER_IMU_INT2_CTRL          0x0EU   // R/W
#define REGISTER_IMU_WHO_AM_I           0x0FU   // R
#define REGISTER_IMU_CTRL1              0x10U   // R/W
#define REGISTER_IMU_CTRL2              0x11U   // R/W
#define REGISTER_IMU_CTRL3              0x12U   // R/W
#define REGISTER_IMU_CTRL4              0x13U   // R/W
#define REGISTER_IMU_CTRL5              0x14U   // R/W
#define REGISTER_IMU_CTRL6              0x15U   // R/W
#define REGISTER_IMU_CTRL7              0x16U   // R/W
#define REGISTER_IMU_CTRL8              0x17U   // R/W
#define REGISTER_IMU_CTRL9              0x18U   // R/W
#define REGISTER_IMU_CTRL10             0x19U   // R/W
#define REGISTER_IMU_CTRL_STATUS        0x1AU   // R/W
#define REGISTER_IMU_FIFO_STATUS1       0x1BU   // R
#define REGISTER_IMU_FIFO_STATUS2       0x1CU   // R
#define REGISTER_IMU_ALL_INT_SRC        0x1DU   // R
#define REGISTER_IMU_STATUS_REG         0x1EU   // R
#define REGISTER_IMU_OUT_TEMP_LSB       0x20U   // R
#define REGISTER_IMU_OUT_TEMP_MSB       0x21U   // R
#define REGISTER_IMU_OUTX_LSB_G         0x22U   // R
#define REGISTER_IMU_OUTX_MSB_G         0x23U   // R
#define REGISTER_IMU_OUTY_LSB_G         0x24U   // R
#define REGISTER_IMU_OUTY_MSB_G         0x25U   // R
#define REGISTER_IMU_OUTZ_LSB_G         0x26U   // R
#define REGISTER_IMU_OUTZ_MSB_G         0x27U   // R
#define REGISTER_IMU_OUTX_LSB_A         0x28U   // R
#define REGISTER_IMU_OUTX_MSB_A         0x29U   // R
#define REGISTER_IMU_OUTY_LSB_A         0x2AU   // R
#define REGISTER_IMU_OUTY_MSB_A         0x2BU   // R
#define REGISTER_IMU_OUTZ_LSB_A         0x2CU   // R
#define REGISTER_IMU_OUTZ_MSB_A         0x2DU   // R
#define REGISTER_IMU_TIMESTAMP0         0x40U   // R
#define REGISTER_IMU_TIMESTAMP1         0x41U   // R
#define REGISTER_IMU_TIMESTAMP2         0x42U   // R
#define REGISTER_IMU_TIMESTAMP3         0x43U   // R
#define REGISTER_IMU_INTERNAL_FREQ      0x4FU   // R
#define REGISTER_IMU_FUNCTIONS_ENABLE   0x50U   // R/W
#define REGISTER_IMU_TAP_CFG0           0x56U   // R/W
#define REGISTER_IMU_MD1_CFG            0x5EU   // R/W
#define REGISTER_IMU_MD2_CFG            0x5FU   // R/W
#define REGISTER_IMU_X_OFS_USR          0x73U   // R/W
#define REGISTER_IMU_Y_OFS_USR          0x74U   // R/W
#define REGISTER_IMU_Z_OFS_USR          0x75U   // R/W
#define REGISTER_IMU_FIFO_DATA_OUT_TAG  0x78U   // R

#define VALUE_IMU_WHO_AM_I              0x70U   // Chip 0x70
#define VALUE_IMU_SW_RESET              0x01U   // Reset software
#define VALUE_IMU_IF_INC                0x04U   // Auto-increment on
#define VALUE_IMU_BDU                   0x40U   // Block-update on
#define VALUE_IMU_BOOT                  0x80U   // Reboot memory
#define VALUE_IMU_ODR_XL_OFF            0x00U   // Rate off
#define VALUE_IMU_ODR_XL_1_875          0x01U   // Rate 1.875Hz
#define VALUE_IMU_ODR_XL_7_5            0x02U   // Rate 7.5Hz
#define VALUE_IMU_ODR_XL_15             0x03U   // Rate 15Hz
#define VALUE_IMU_ODR_XL_30             0x04U   // Rate 30Hz
#define VALUE_IMU_ODR_XL_60             0x05U   // Rate 60Hz
#define VALUE_IMU_ODR_XL_120            0x06U   // Rate 120Hz
#define VALUE_IMU_ODR_XL_240            0x07U   // Rate 240Hz
#define VALUE_IMU_ODR_XL_480            0x08U   // Rate 480Hz
#define VALUE_IMU_ODR_XL_960            0x09U   // Rate 960Hz
#define VALUE_IMU_ODR_XL_1920           0x0AU   // Rate 1920Hz
#define VALUE_IMU_ODR_XL_3840           0x0BU   // Rate 3840Hz
#define VALUE_IMU_ODR_XL_7680           0x0CU   // Rate 7680Hz
#define VALUE_IMU_MODE_XL_HP            0x00U   // Mode high-performance
#define VALUE_IMU_MODE_XL_HAODR         0x10U   // Mode high-accuracy
#define VALUE_IMU_MODE_XL_TRIG          0x30U   // Mode ODR-triggered
#define VALUE_IMU_MODE_XL_LP1           0x40U   // Mode low-power-1
#define VALUE_IMU_MODE_XL_LP2           0x50U   // Mode low-power-2
#define VALUE_IMU_MODE_XL_LP3           0x60U   // Mode low-power-3
#define VALUE_IMU_MODE_XL_NORMAL        0x70U   // Mode normal
#define VALUE_IMU_ODR_G_OFF             0x00U   // Rate off
#define VALUE_IMU_ODR_G_7_5             0x02U   // Rate 7.5Hz
#define VALUE_IMU_ODR_G_15              0x03U   // Rate 15Hz
#define VALUE_IMU_ODR_G_30              0x04U   // Rate 30Hz
#define VALUE_IMU_ODR_G_60              0x05U   // Rate 60Hz
#define VALUE_IMU_ODR_G_120             0x06U   // Rate 120Hz
#define VALUE_IMU_ODR_G_240             0x07U   // Rate 240Hz
#define VALUE_IMU_ODR_G_480             0x08U   // Rate 480Hz
#define VALUE_IMU_ODR_G_960             0x09U   // Rate 960Hz
#define VALUE_IMU_ODR_G_1920            0x0AU   // Rate 1920Hz
#define VALUE_IMU_ODR_G_3840            0x0BU   // Rate 3840Hz
#define VALUE_IMU_ODR_G_7680            0x0CU   // Rate 7680Hz
#define VALUE_IMU_MODE_G_HP             0x00U   // Mode high-performance
#define VALUE_IMU_MODE_G_HAODR          0x10U   // Mode high-accuracy
#define VALUE_IMU_MODE_G_SLEEP          0x40U   // Mode sleep
#define VALUE_IMU_MODE_G_LP             0x50U   // Mode low-power
#define VALUE_IMU_CTRL8_RESERVED        0x04U   // Reserved 0x04
#define VALUE_IMU_FS_XL_4               0x00U   // Range 4g
#define VALUE_IMU_FS_XL_8               0x01U   // Range 8g
#define VALUE_IMU_FS_XL_16              0x02U   // Range 16g
#define VALUE_IMU_FS_XL_32              0x03U   // Range 32g
#define VALUE_IMU_XL_DUALC              0x08U   // Dual-channel on
#define VALUE_IMU_LPF2_XL_4             0x00U   // Bandwidth ODR/4
#define VALUE_IMU_LPF2_XL_10            0x20U   // Bandwidth ODR/10
#define VALUE_IMU_LPF2_XL_20            0x40U   // Bandwidth ODR/20
#define VALUE_IMU_LPF2_XL_45            0x60U   // Bandwidth ODR/45
#define VALUE_IMU_LPF2_XL_100           0x80U   // Bandwidth ODR/100
#define VALUE_IMU_LPF2_XL_200           0xA0U   // Bandwidth ODR/200
#define VALUE_IMU_LPF2_XL_400           0xC0U   // Bandwidth ODR/400
#define VALUE_IMU_LPF2_XL_800           0xE0U   // Bandwidth ODR/800
#define VALUE_IMU_FS_G_125              0x00U   // Range 125dps
#define VALUE_IMU_FS_G_250              0x01U   // Range 250dps
#define VALUE_IMU_FS_G_500              0x02U   // Range 500dps
#define VALUE_IMU_FS_G_1000             0x03U   // Range 1000dps
#define VALUE_IMU_FS_G_2000             0x04U   // Range 2000dps
#define VALUE_IMU_FS_G_4000             0x0CU   // Range 4000dps
#define VALUE_IMU_LPF1_G_0              0x00U   // Bandwidth 175Hz@480Hz
#define VALUE_IMU_LPF1_G_1              0x10U   // Bandwidth 157Hz@480Hz
#define VALUE_IMU_LPF1_G_2              0x20U   // Bandwidth 131Hz@480Hz
#define VALUE_IMU_LPF1_G_3              0x30U   // Bandwidth 188Hz@480Hz
#define VALUE_IMU_LPF1_G_4              0x40U   // Bandwidth 94Hz@480Hz
#define VALUE_IMU_LPF1_G_5              0x50U   // Bandwidth 56.7Hz@480Hz
#define VALUE_IMU_LPF1_G_6              0x60U   // Bandwidth 28.4Hz@480Hz
#define VALUE_IMU_LPF1_G_7              0x70U   // Bandwidth 14.3Hz@480Hz
#define VALUE_IMU_LPF1_G_EN             0x01U   // LPF1 on
#define VALUE_IMU_USR_OFF_ON_OUT        0x01U   // Offset on
#define VALUE_IMU_USR_OFF_W             0x02U   // Weight 2^-6g
#define VALUE_IMU_LPF2_XL_EN            0x08U   // LPF2 on
#define VALUE_IMU_HP_SLOPE_XL_EN        0x10U   // Highpass on
#define VALUE_IMU_XL_FASTSETTL          0x20U   // Fast-settle on
#define VALUE_IMU_HP_REF_MODE_XL        0x40U   // Reference on
#define VALUE_IMU_DRDY_PULSED           0x02U   // Data-ready pulsed
#define VALUE_IMU_DRDY_MASK             0x08U   // Data-ready masked
#define VALUE_IMU_INT1_DRDY_XL          0x01U   // Interrupt accel-ready
#define VALUE_IMU_INT1_DRDY_G           0x02U   // Interrupt gyro-ready
#define VALUE_IMU_INT1_FIFO_TH          0x08U   // Interrupt FIFO-threshold
#define VALUE_IMU_INT1_FIFO_OVR         0x10U   // Interrupt FIFO-overrun
#define VALUE_IMU_INT1_FIFO_FULL        0x20U   // Interrupt FIFO-full
#define VALUE_IMU_INT1_CNT_BDR          0x40U   // Interrupt batch-counter
#define VALUE_IMU_ST_XL_POSITIVE        0x01U   // Self-test positive
#define VALUE_IMU_ST_XL_NEGATIVE        0x02U   // Self-test negative
#define VALUE_IMU_ST_G_POSITIVE         0x04U   // Self-test positive
#define VALUE_IMU_ST_G_NEGATIVE         0x08U   // Self-test negative

#endif // LSM5DSV32X_H