/**
 * @file    config.h
 * @brief   Libraries: Standard, PI SDK, and internal.
 *          Addresses: IMU, barometer.
 *          OPCODES: Flash, GNSS, SD, LORA.
 *          Helpers
 * @author  esicoe
 */

#ifndef CONFIG_H
#define CONFIG_H

// STANDARD LIBRARY
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// RASPBERRY PI SDK
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/interp.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"

// INTERNAL LIBRARY
#include "I2C.h"
#include "LED.h"
#include "BMP585.h"

// PINOUT
#define PIN_SPI0_MOSI                               19U         // PIN 19
#define PIN_SPI0_MISO                               16U         // PIN 16
#define PIN_SPI0_SCK                                18U         // PIN 18
#define PIN_SPI0_CS                                 17U         // PIN 17

#define PIN_SPI1_MOSI                               31U         // PIN 39
#define PIN_SPI1_MISO                               28U         // PIN 36
#define PIN_SPI1_SCK                                30U         // PIN 38
#define PIN_SPI1_CS                                 29U         // PIN 37

#define PIN_I2C0_SCL                                9U          // PIN 7
#define PIN_I2C0_SDA                                8U          // PIN 6
#define PIN_IMU_INT                                 7U          // PIN 4

#define PIN_I2C1_SCL                                11U         // PIN 9
#define PIN_I2C1_SDA                                10U         // PIN 8

#define PIN_UART0_TX                                0U          // PIN 77
#define PIN_UART0_RX                                1U          // PIN 78
#define PIN_GNSS_RST                                2U          // PIN 79

#define PIN_UART1_TX                                24U         // PIN 25
#define PIN_UART1_RX                                25U         // PIN 26
#define PIN_LORA_RST                                20U         // PIN 20

#define PIN_SERVO1_PWM                              32U         // PIN 40
#define PIN_SERVO2_PWM                              36U         // PIN 45

#define PIN_PYRO1                                   43U         // PIN 54
#define PIN_PYRO2                                   44U         // PIN 55
#define PIN_PYRO3                                   45U         // PIN 56

#define PIN_LED_STATUSA                             12U         // PIN 11
#define PIN_LED_STATUSB                             13U         // PIN 12
#define PIN_LED_STATUSC                             14U         // PIN 13

// HELPERS
#define PYRO_OFF                                    false
#define PYRO_ON                                     true
#define LED_OFF                                     false
#define LED_ON                                      true

// CLOCKS           
#define I2C0_CLOCK                                  400000UL    // 400 KHz
#define I2C1_CLOCK                                  400000UL    // 400 KHz
#define SPI0_CLOCK                                  20000000UL  // 20 MHz
#define SPI1_CLOCK                                  400000UL    // 400 KHz
#define UART0_BAUD                                  9600UL      // 9.6 KBd
#define UART1_BAUD                                  115200UL    // 115.2 Bd

// ADDRESSES            
#define ADDRESS_IMU_I2C                             0x6AU       // W
#define ADDRESS_IMU_FUNC_CFG_ACCESS                 0x01U       // R/W
#define ADDRESS_IMU_PIN_CTRL                        0x02U       // R/W
#define ADDRESS_IMU_IF_CFG                          0x03U       // R/W
#define ADDRESS_IMU_FIFO_CTRL1                      0x07U       // R/W
#define ADDRESS_IMU_FIFO_CTRL2                      0x08U       // R/W
#define ADDRESS_IMU_FIFO_CTRL3                      0x09U       // R/W
#define ADDRESS_IMU_FIFO_CTRL4                      0x0AU       // R/W
#define ADDRESS_IMU_COUNTER_BDR_REG1                0x0BU       // R/W
#define ADDRESS_IMU_INT1_CTRL                       0x0DU       // R/W
#define ADDRESS_IMU_INT2_CTRL                       0x0EU       // R/W
#define ADDRESS_IMU_WHO_AM_I                        0x0FU       // R
#define ADDRESS_IMU_CTRL1                           0x10U       // R/W
#define ADDRESS_IMU_CTRL2                           0x11U       // R/W
#define ADDRESS_IMU_CTRL3                           0x12U       // R/W
#define ADDRESS_IMU_CTRL4                           0x13U       // R/W
#define ADDRESS_IMU_CTRL5                           0x14U       // R/W
#define ADDRESS_IMU_CTRL6                           0x15U       // R/W
#define ADDRESS_IMU_CTRL7                           0x16U       // R/W
#define ADDRESS_IMU_CTRL8                           0x17U       // R/W
#define ADDRESS_IMU_CTRL9                           0x18U       // R/W
#define ADDRESS_IMU_CTRL10                          0x19U       // R/W
#define ADDRESS_IMU_CTRL_STATUS                     0x1AU       // R/W
#define ADDRESS_IMU_FIFO_STATUS1                    0x1BU       // R
#define ADDRESS_IMU_FIFO_STATUS2                    0x1CU       // R
#define ADDRESS_IMU_ALL_INT_SRC                     0x1DU       // R
#define ADDRESS_IMU_STATUS_REG                      0x1EU       // R
#define ADDRESS_IMU_OUT_TEMP_LSB                    0x20U       // R
#define ADDRESS_IMU_OUT_TEMP_MSB                    0x21U       // R
#define ADDRESS_IMU_OUTX_LSB_G                      0x22U       // R
#define ADDRESS_IMU_OUTX_MSB_G                      0x23U       // R
#define ADDRESS_IMU_OUTY_LSB_G                      0x24U       // R
#define ADDRESS_IMU_OUTY_MSB_G                      0x25U       // R
#define ADDRESS_IMU_OUTZ_LSB_G                      0x26U       // R
#define ADDRESS_IMU_OUTZ_MSB_G                      0x27U       // R
#define ADDRESS_IMU_OUTX_LSB_A                      0x28U       // R
#define ADDRESS_IMU_OUTX_MSB_A                      0x29U       // R
#define ADDRESS_IMU_OUTY_LSB_A                      0x2AU       // R
#define ADDRESS_IMU_OUTY_MSB_A                      0x2BU       // R
#define ADDRESS_IMU_OUTZ_LSB_A                      0x2CU       // R
#define ADDRESS_IMU_OUTZ_MSB_A                      0x2DU       // R
#define ADDRESS_IMU_TIMESTAMP0                      0x40U       // R
#define ADDRESS_IMU_TIMESTAMP1                      0x41U       // R
#define ADDRESS_IMU_TIMESTAMP2                      0x42U       // R
#define ADDRESS_IMU_TIMESTAMP3                      0x43U       // R
#define ADDRESS_IMU_INTERNAL_FREQ                   0x4FU       // R
#define ADDRESS_IMU_FUNCTIONS_ENABLE                0x50U       // R/W
#define ADDRESS_IMU_TAP_CFG0                        0x56U       // R/W
#define ADDRESS_IMU_MD1_CFG                         0x5EU       // R/W
#define ADDRESS_IMU_MD2_CFG                         0x5FU       // R/W
#define ADDRESS_IMU_X_OFS_USR                       0x73U       // R/W
#define ADDRESS_IMU_Y_OFS_USR                       0x74U       // R/W
#define ADDRESS_IMU_Z_OFS_USR                       0x75U       // R/W
#define ADDRESS_IMU_FIFO_DATA_OUT_TAG               0x78U       // R

#define ADDRESS_BARO_I2C                            0x46U       //
#define ADDRESS_BARO_CHIP_ID                        0x01U       // R
#define ADDRESS_BARO_REV_ID                         0x02U       // R
#define ADDRESS_BARO_CHIP_STATUS                    0x11U       // R
#define ADDRESS_BARO_DRIVE_CONFIG                   0x13U       // R/W
#define ADDRESS_BARO_INT_CONFIG                     0x14U       // R/W
#define ADDRESS_BARO_INT_SOURCE                     0x15U       // R/W
#define ADDRESS_BARO_FIFO_CONFIG                    0x16U       // R/W
#define ADDRESS_BARO_FIFO_COUNT                     0x17U       // R
#define ADDRESS_BARO_FIFO_SEL                       0x18U       // R/W
#define ADDRESS_BARO_TEMP_DATA_XLSB                 0x1DU       // R
#define ADDRESS_BARO_TEMP_DATA_LSB                  0x1EU       // R
#define ADDRESS_BARO_TEMP_DATA_MSB                  0x1FU       // R
#define ADDRESS_BARO_PRESS_DATA_XLSB                0x20U       // R
#define ADDRESS_BARO_PRESS_DATA_LSB                 0x21U       // R
#define ADDRESS_BARO_PRESS_DATA_MSB                 0x22U       // R
#define ADDRESS_BARO_INT_STATUS                     0x27U       // R
#define ADDRESS_BARO_STATUS                         0x28U       // R
#define ADDRESS_BARO_FIFO_DATA                      0x29U       // R
#define ADDRESS_BARO_NVM_ADDR                       0x2BU       // R/W
#define ADDRESS_BARO_NVM_DATA_LSB                   0x2CU       // R/W
#define ADDRESS_BARO_NVM_DATA_MSB                   0x2DU       // R/W
#define ADDRESS_BARO_DSP_CONFIG                     0x30U       // R/W
#define ADDRESS_BARO_DSP_IIR                        0x31U       // R/W
#define ADDRESS_BARO_OOR_THR_P_LSB                  0x32U       // R/W
#define ADDRESS_BARO_OOR_THR_P_MSB                  0x33U       // R/W
#define ADDRESS_BARO_OOR_RANGE                      0x34U       // R/W
#define ADDRESS_BARO_OOR_CONFIG                     0x35U       // R/W
#define ADDRESS_BARO_OSR_CONFIG                     0x36U       // R/W
#define ADDRESS_BARO_ODR_CONFIG                     0x37U       // R/W
#define ADDRESS_BARO_OSR_EFF                        0x38U       // R
#define ADDRESS_BARO_CMD                            0x7EU       // R/W

// REGISTER VALUES
#define VALUE_IMU_WHO_AM_I                          0x70U       // Chip 0x70
#define VALUE_IMU_SW_RESET                          0x01U       // Reset software
#define VALUE_IMU_IF_INC                            0x04U       // Auto-increment on
#define VALUE_IMU_BDU                               0x40U       // Block-update on
#define VALUE_IMU_BOOT                              0x80U       // Reboot memory
#define VALUE_IMU_ODR_XL_OFF                        0x00U       // Rate off
#define VALUE_IMU_ODR_XL_1_875                      0x01U       // Rate 1.875Hz
#define VALUE_IMU_ODR_XL_7_5                        0x02U       // Rate 7.5Hz
#define VALUE_IMU_ODR_XL_15                         0x03U       // Rate 15Hz
#define VALUE_IMU_ODR_XL_30                         0x04U       // Rate 30Hz
#define VALUE_IMU_ODR_XL_60                         0x05U       // Rate 60Hz
#define VALUE_IMU_ODR_XL_120                        0x06U       // Rate 120Hz
#define VALUE_IMU_ODR_XL_240                        0x07U       // Rate 240Hz
#define VALUE_IMU_ODR_XL_480                        0x08U       // Rate 480Hz
#define VALUE_IMU_ODR_XL_960                        0x09U       // Rate 960Hz
#define VALUE_IMU_ODR_XL_1920                       0x0AU       // Rate 1920Hz
#define VALUE_IMU_ODR_XL_3840                       0x0BU       // Rate 3840Hz
#define VALUE_IMU_ODR_XL_7680                       0x0CU       // Rate 7680Hz
#define VALUE_IMU_MODE_XL_HP                        0x00U       // Mode high-performance
#define VALUE_IMU_MODE_XL_HAODR                     0x10U       // Mode high-accuracy
#define VALUE_IMU_MODE_XL_TRIG                      0x30U       // Mode ODR-triggered
#define VALUE_IMU_MODE_XL_LP1                       0x40U       // Mode low-power-1
#define VALUE_IMU_MODE_XL_LP2                       0x50U       // Mode low-power-2
#define VALUE_IMU_MODE_XL_LP3                       0x60U       // Mode low-power-3
#define VALUE_IMU_MODE_XL_NORMAL                    0x70U       // Mode normal
#define VALUE_IMU_ODR_G_OFF                         0x00U       // Rate off
#define VALUE_IMU_ODR_G_7_5                         0x02U       // Rate 7.5Hz
#define VALUE_IMU_ODR_G_15                          0x03U       // Rate 15Hz
#define VALUE_IMU_ODR_G_30                          0x04U       // Rate 30Hz
#define VALUE_IMU_ODR_G_60                          0x05U       // Rate 60Hz
#define VALUE_IMU_ODR_G_120                         0x06U       // Rate 120Hz
#define VALUE_IMU_ODR_G_240                         0x07U       // Rate 240Hz
#define VALUE_IMU_ODR_G_480                         0x08U       // Rate 480Hz
#define VALUE_IMU_ODR_G_960                         0x09U       // Rate 960Hz
#define VALUE_IMU_ODR_G_1920                        0x0AU       // Rate 1920Hz
#define VALUE_IMU_ODR_G_3840                        0x0BU       // Rate 3840Hz
#define VALUE_IMU_ODR_G_7680                        0x0CU       // Rate 7680Hz
#define VALUE_IMU_MODE_G_HP                         0x00U       // Mode high-performance
#define VALUE_IMU_MODE_G_HAODR                      0x10U       // Mode high-accuracy
#define VALUE_IMU_MODE_G_SLEEP                      0x40U       // Mode sleep
#define VALUE_IMU_MODE_G_LP                         0x50U       // Mode low-power
#define VALUE_IMU_CTRL8_RESERVED                    0x04U       // Reserved 0x04
#define VALUE_IMU_FS_XL_4                           0x00U       // Range 4g
#define VALUE_IMU_FS_XL_8                           0x01U       // Range 8g
#define VALUE_IMU_FS_XL_16                          0x02U       // Range 16g
#define VALUE_IMU_FS_XL_32                          0x03U       // Range 32g
#define VALUE_IMU_XL_DUALC                          0x08U       // Dual-channel on
#define VALUE_IMU_LPF2_XL_4                         0x00U       // Bandwidth ODR/4
#define VALUE_IMU_LPF2_XL_10                        0x20U       // Bandwidth ODR/10
#define VALUE_IMU_LPF2_XL_20                        0x40U       // Bandwidth ODR/20
#define VALUE_IMU_LPF2_XL_45                        0x60U       // Bandwidth ODR/45
#define VALUE_IMU_LPF2_XL_100                       0x80U       // Bandwidth ODR/100
#define VALUE_IMU_LPF2_XL_200                       0xA0U       // Bandwidth ODR/200
#define VALUE_IMU_LPF2_XL_400                       0xC0U       // Bandwidth ODR/400
#define VALUE_IMU_LPF2_XL_800                       0xE0U       // Bandwidth ODR/800
#define VALUE_IMU_FS_G_125                          0x00U       // Range 125dps
#define VALUE_IMU_FS_G_250                          0x01U       // Range 250dps
#define VALUE_IMU_FS_G_500                          0x02U       // Range 500dps
#define VALUE_IMU_FS_G_1000                         0x03U       // Range 1000dps
#define VALUE_IMU_FS_G_2000                         0x04U       // Range 2000dps
#define VALUE_IMU_FS_G_4000                         0x0CU       // Range 4000dps
#define VALUE_IMU_LPF1_G_0                          0x00U       // Bandwidth 175Hz@480Hz
#define VALUE_IMU_LPF1_G_1                          0x10U       // Bandwidth 157Hz@480Hz
#define VALUE_IMU_LPF1_G_2                          0x20U       // Bandwidth 131Hz@480Hz
#define VALUE_IMU_LPF1_G_3                          0x30U       // Bandwidth 188Hz@480Hz
#define VALUE_IMU_LPF1_G_4                          0x40U       // Bandwidth 94Hz@480Hz
#define VALUE_IMU_LPF1_G_5                          0x50U       // Bandwidth 56.7Hz@480Hz
#define VALUE_IMU_LPF1_G_6                          0x60U       // Bandwidth 28.4Hz@480Hz
#define VALUE_IMU_LPF1_G_7                          0x70U       // Bandwidth 14.3Hz@480Hz
#define VALUE_IMU_LPF1_G_EN                         0x01U       // LPF1 on
#define VALUE_IMU_USR_OFF_ON_OUT                    0x01U       // Offset on
#define VALUE_IMU_USR_OFF_W                         0x02U       // Weight 2^-6g
#define VALUE_IMU_LPF2_XL_EN                        0x08U       // LPF2 on
#define VALUE_IMU_HP_SLOPE_XL_EN                    0x10U       // Highpass on
#define VALUE_IMU_XL_FASTSETTL                      0x20U       // Fast-settle on
#define VALUE_IMU_HP_REF_MODE_XL                    0x40U       // Reference on
#define VALUE_IMU_DRDY_PULSED                       0x02U       // Data-ready pulsed
#define VALUE_IMU_DRDY_MASK                         0x08U       // Data-ready masked
#define VALUE_IMU_INT1_DRDY_XL                      0x01U       // Interrupt accel-ready
#define VALUE_IMU_INT1_DRDY_G                       0x02U       // Interrupt gyro-ready
#define VALUE_IMU_INT1_FIFO_TH                      0x08U       // Interrupt FIFO-threshold
#define VALUE_IMU_INT1_FIFO_OVR                     0x10U       // Interrupt FIFO-overrun
#define VALUE_IMU_INT1_FIFO_FULL                    0x20U       // Interrupt FIFO-full
#define VALUE_IMU_INT1_CNT_BDR                      0x40U       // Interrupt batch-counter
#define VALUE_IMU_ST_XL_POSITIVE                    0x01U       // Self-test positive
#define VALUE_IMU_ST_XL_NEGATIVE                    0x02U       // Self-test negative
#define VALUE_IMU_ST_G_POSITIVE                     0x04U       // Self-test positive
#define VALUE_IMU_ST_G_NEGATIVE                     0x08U       // Self-test negative
#define VALUE_BARO_CHIP_ID                          0x51U       // Chip 0x51
#define VALUE_BARO_STATUS_RDY                       0x02U       // Chip ready
#define VALUE_BARO_CMD_SOFT_RESET                   0xB6U       // Command reset
#define VALUE_BARO_ODR_IS_VALID                     0x80U       // ODR valid
#define VALUE_BARO_PRESS_OFF                        0x00U       // Pressure off
#define VALUE_BARO_PRESS_ON                         0x40U       // Pressure on
#define VALUE_BARO_OSR_P_1                          0x00U       // Oversampling x1
#define VALUE_BARO_OSR_P_2                          0x08U       // Oversampling x2
#define VALUE_BARO_OSR_P_4                          0x10U       // Oversampling x4
#define VALUE_BARO_OSR_P_8                          0x18U       // Oversampling x8
#define VALUE_BARO_OSR_P_16                         0x20U       // Oversampling x16
#define VALUE_BARO_OSR_P_32                         0x28U       // Oversampling x32
#define VALUE_BARO_OSR_P_64                         0x30U       // Oversampling x64
#define VALUE_BARO_OSR_P_128                        0x38U       // Oversampling x128
#define VALUE_BARO_OSR_T_1                          0x00U       // Oversampling x1
#define VALUE_BARO_OSR_T_2                          0x01U       // Oversampling x2
#define VALUE_BARO_OSR_T_4                          0x02U       // Oversampling x4
#define VALUE_BARO_OSR_T_8                          0x03U       // Oversampling x8
#define VALUE_BARO_OSR_T_16                         0x04U       // Oversampling x16
#define VALUE_BARO_OSR_T_32                         0x05U       // Oversampling x32
#define VALUE_BARO_OSR_T_64                         0x06U       // Oversampling x64
#define VALUE_BARO_OSR_T_128                        0x07U       // Oversampling x128
#define VALUE_BARO_IIR_P_0                          0x00U       // Coefficient bypass
#define VALUE_BARO_IIR_P_1                          0x08U       // Coefficient 1
#define VALUE_BARO_IIR_P_3                          0x10U       // Coefficient 3
#define VALUE_BARO_IIR_P_7                          0x18U       // Coefficient 7
#define VALUE_BARO_IIR_P_15                         0x20U       // Coefficient 15
#define VALUE_BARO_IIR_P_31                         0x28U       // Coefficient 31
#define VALUE_BARO_IIR_P_63                         0x30U       // Coefficient 63
#define VALUE_BARO_IIR_P_127                        0x38U       // Coefficient 127
#define VALUE_BARO_IIR_T_0                          0x00U       // Coefficient bypass
#define VALUE_BARO_IIR_T_1                          0x01U       // Coefficient 1
#define VALUE_BARO_IIR_T_3                          0x02U       // Coefficient 3
#define VALUE_BARO_IIR_T_7                          0x03U       // Coefficient 7
#define VALUE_BARO_IIR_T_15                         0x04U       // Coefficient 15
#define VALUE_BARO_IIR_T_31                         0x05U       // Coefficient 31
#define VALUE_BARO_IIR_T_63                         0x06U       // Coefficient 63
#define VALUE_BARO_IIR_T_127                        0x07U       // Coefficient 127
#define VALUE_BARO_DSP_RESERVED                     0x03U       // Reserved 0x03
#define VALUE_BARO_DSP_IIR_FLUSH                    0x04U       // Flush forced
#define VALUE_BARO_DSP_SHDW_IIR_T                   0x08U       // Filtered temperature
#define VALUE_BARO_DSP_FIFO_IIR_T                   0x10U       // Filtered FIFO-temperature
#define VALUE_BARO_DSP_SHDW_IIR_P                   0x20U       // Filtered pressure
#define VALUE_BARO_DSP_FIFO_IIR_P                   0x40U       // Filtered FIFO-pressure
#define VALUE_BARO_DSP_OOR_IIR_P                    0x80U       // Filtered out-of-range
#define VALUE_BARO_INT_DRDY                         0x01U       // Interrupt data-ready
#define VALUE_BARO_INT_FIFO_FULL                    0x02U       // Interrupt FIFO-full
#define VALUE_BARO_INT_FIFO_THS                     0x04U       // Interrupt FIFO-threshold
#define VALUE_BARO_INT_OOR_P                        0x08U       // Interrupt out-of-range
#define VALUE_BARO_MODE_STANDBY                     0x00U       // Mode standby
#define VALUE_BARO_MODE_NORMAL                      0x01U       // Mode normal
#define VALUE_BARO_MODE_FORCED                      0x02U       // Mode forced
#define VALUE_BARO_MODE_NONSTOP                     0x03U       // Mode non-stop
#define VALUE_BARO_ODR_240                          0x00U       // Rate 240Hz
#define VALUE_BARO_ODR_218                          0x04U       // Rate 218.537Hz
#define VALUE_BARO_ODR_199                          0x08U       // Rate 199.111Hz
#define VALUE_BARO_ODR_179                          0x0CU       // Rate 179.2Hz
#define VALUE_BARO_ODR_160                          0x10U       // Rate 160Hz
#define VALUE_BARO_ODR_149                          0x14U       // Rate 149.333Hz
#define VALUE_BARO_ODR_140                          0x18U       // Rate 140Hz
#define VALUE_BARO_ODR_130                          0x1CU       // Rate 129.855Hz
#define VALUE_BARO_ODR_120                          0x20U       // Rate 120Hz
#define VALUE_BARO_ODR_110                          0x24U       // Rate 110.164Hz
#define VALUE_BARO_ODR_100                          0x28U       // Rate 100.299Hz
#define VALUE_BARO_ODR_90                           0x2CU       // Rate 89.6Hz
#define VALUE_BARO_ODR_80                           0x30U       // Rate 80Hz
#define VALUE_BARO_ODR_70                           0x34U       // Rate 70Hz
#define VALUE_BARO_ODR_60                           0x38U       // Rate 60Hz
#define VALUE_BARO_ODR_50                           0x3CU       // Rate 50.056Hz
#define VALUE_BARO_ODR_45                           0x40U       // Rate 45.025Hz
#define VALUE_BARO_ODR_40                           0x44U       // Rate 40Hz
#define VALUE_BARO_ODR_35                           0x48U       // Rate 35Hz
#define VALUE_BARO_ODR_30                           0x4CU       // Rate 30Hz
#define VALUE_BARO_ODR_25                           0x50U       // Rate 25.005Hz
#define VALUE_BARO_ODR_20                           0x54U       // Rate 20Hz
#define VALUE_BARO_ODR_15                           0x58U       // Rate 15Hz
#define VALUE_BARO_ODR_10                           0x5CU       // Rate 10Hz
#define VALUE_BARO_ODR_5                            0x60U       // Rate 5Hz
#define VALUE_BARO_ODR_4                            0x64U       // Rate 4Hz
#define VALUE_BARO_ODR_3                            0x68U       // Rate 3Hz
#define VALUE_BARO_ODR_2                            0x6CU       // Rate 2Hz
#define VALUE_BARO_ODR_1                            0x70U       // Rate 1Hz
#define VALUE_BARO_ODR_0_5                          0x74U       // Rate 0.5Hz
#define VALUE_BARO_ODR_0_25                         0x78U       // Rate 0.25Hz
#define VALUE_BARO_ODR_0_125                        0x7CU       // Rate 0.125Hz
#define VALUE_BARO_DEEP_DISABLE                     0x80U       // Deep-standby off

// OPCODES
#define OPCODE_FLASH_WRITE_ENABLE                   0x06U
#define OPCODE_FLASH_VOLATILE_SR_WRITE_ENABLE       0x50U
#define OPCODE_FLASH_WRITE_DISABLE                  0x04U
#define OPCODE_FLASH_RELEASE_POWER_DOWN_ID          0xABU
#define OPCODE_FLASH_MANUFACTURER_DEVICE_ID         0x90U
#define OPCODE_FLASH_JEDEC_ID                       0x9FU
#define OPCODE_FLASH_READ_UNIQUE_ID                 0x4BU
#define OPCODE_FLASH_READ_DATA                      0x03U
#define OPCODE_FLASH_FAST_READ                      0x0BU
#define OPCODE_FLASH_PAGE_PROGRAM                   0x02U
#define OPCODE_FLASH_SECTOR_ERASE_4KB               0x20U
#define OPCODE_FLASH_BLOCK_ERASE_32KB               0x52U
#define OPCODE_FLASH_BLOCK_ERASE_64KB               0xD8U
#define OPCODE_FLASH_CHIP_ERASE                     0xC7U
#define OPCODE_FLASH_READ_STATUS_REGISTER_1         0x05U
#define OPCODE_FLASH_WRITE_STATUS_REGISTER_1        0x01U
#define OPCODE_FLASH_READ_STATUS_REGISTER_2         0x35U
#define OPCODE_FLASH_WRITE_STATUS_REGISTER_2        0x31U
#define OPCODE_FLASH_READ_STATUS_REGISTER_3         0x15U
#define OPCODE_FLASH_WRITE_STATUS_REGISTER_3        0x11U
#define OPCODE_FLASH_READ_SFDP_REGISTER             0x5AU
#define OPCODE_FLASH_ERASE_SECURITY_REGISTER        0x44U
#define OPCODE_FLASH_PROGRAM_SECURITY_REGISTER      0x42U
#define OPCODE_FLASH_READ_SECURITY_REGISTER         0x48U
#define OPCODE_FLASH_GLOBAL_BLOCK_LOCK              0x7EU
#define OPCODE_FLASH_GLOBAL_BLOCK_UNLOCK            0x98U
#define OPCODE_FLASH_READ_BLOCK_LOCK                0x3DU
#define OPCODE_FLASH_INDIVIDUAL_BLOCK_LOCK          0x36U
#define OPCODE_FLASH_INDIVIDUAL_BLOCK_UNLOCK        0x39U
#define OPCODE_FLASH_ERASE_PROGRAM_SUSPEND          0x75U
#define OPCODE_FLASH_ERASE_PROGRAM_RESUME           0x7AU
#define OPCODE_FLASH_POWER_DOWN                     0xB9U
#define OPCODE_FLASH_ENABLE_RESET                   0x66U
#define OPCODE_FLASH_RESET_DEVICE                   0x99U

#define OPCODE_SD_GO_IDLE_STATE                     0U
#define OPCODE_SD_SEND_IF_COND                      8U
#define OPCODE_SD_SEND_CSD                          9U
#define OPCODE_SD_SEND_CID                          10U
#define OPCODE_SD_STOP_TRANSMISSION                 12U
#define OPCODE_SD_SEND_STATUS                       13U
#define OPCODE_SD_SET_BLOCKLEN                      16U
#define OPCODE_SD_READ_SINGLE_BLOCK                 17U
#define OPCODE_SD_READ_MULTIPLE_BLOCK               18U
#define OPCODE_SD_WRITE_BLOCK                       24U
#define OPCODE_SD_WRITE_MULTIPLE_BLOCK              25U
#define OPCODE_SD_ERASE_WR_BLK_START_ADDR           32U
#define OPCODE_SD_ERASE_WR_BLK_END_ADDR             33U
#define OPCODE_SD_ERASE                             38U
#define OPCODE_SD_APP_CMD                           55U
#define OPCODE_SD_READ_OCR                          58U
#define OPCODE_SD_CRC_ON_OFF                        59U
#define OPCODE_SD_APP_SD_STATUS                     13U
#define OPCODE_SD_APP_SET_WR_BLK_ERASE_COUNT        23U
#define OPCODE_SD_APP_SD_SEND_OP_COND               41U
#define OPCODE_SD_APP_SEND_SCR                      51U
#define OPCODE_SD_START_BLOCK_TOKEN                 0xFEU
#define OPCODE_SD_START_MULTI_WRITE_TOKEN           0xFCU
#define OPCODE_SD_STOP_TRAN_TOKEN                   0xFDU

#define OPCODE_GNSS_UBX_SYNC_CHAR_1                 0xB5U
#define OPCODE_GNSS_UBX_SYNC_CHAR_2                 0x62U
#define OPCODE_GNSS_CLASS_NAV                       0x01U
#define OPCODE_GNSS_CLASS_ACK                       0x05U
#define OPCODE_GNSS_CLASS_CFG                       0x06U
#define OPCODE_GNSS_CLASS_MON                       0x0AU
#define OPCODE_GNSS_NAV_STATUS                      0x03U
#define OPCODE_GNSS_NAV_PVT                         0x07U
#define OPCODE_GNSS_NAV_SAT                         0x35U
#define OPCODE_GNSS_ACK_NAK                         0x00U
#define OPCODE_GNSS_ACK_ACK                         0x01U
#define OPCODE_GNSS_CFG_RST                         0x04U
#define OPCODE_GNSS_CFG_VALSET                      0x8AU
#define OPCODE_GNSS_CFG_VALGET                      0x8BU
#define OPCODE_GNSS_CFG_VALDEL                      0x8CU
#define OPCODE_GNSS_MON_VER                         0x04U
#define OPCODE_GNSS_MON_RF                          0x38U

#define OPCODE_LORA_AT                              "AT"
#define OPCODE_LORA_FIRMWARE_VERSION                "AT+VER=?"
#define OPCODE_LORA_NETWORK_MODE                    "AT+NWM="
#define OPCODE_LORA_P2P_PARAMETERS                  "AT+P2P="
#define OPCODE_LORA_P2P_SEND                        "AT+PSEND="
#define OPCODE_LORA_P2P_RECEIVE                     "AT+PRECV="
#define OPCODE_LORA_LOW_POWER_MODE                  "AT+LPM="
#define OPCODE_LORA_BAND                            "AT+BAND="
#define OPCODE_LORA_EVENT_TX_DONE                   "+EVT:TXP2P DONE"
#define OPCODE_LORA_EVENT_RX                        "+EVT:RXP2P"

// SENSOR CONFIGURATION
struct CONFIGURATION {
    uint8_t reg;
    uint8_t value;
};

static const struct CONFIGURATION BMP585[] = {
    {ADDRESS_BARO_DSP_CONFIG, VALUE_BARO_DSP_RESERVED | VALUE_BARO_DSP_SHDW_IIR_P                  },
    {ADDRESS_BARO_DSP_IIR,    VALUE_BARO_IIR_P_3      | VALUE_BARO_IIR_T_0                         },
    {ADDRESS_BARO_OSR_CONFIG, VALUE_BARO_PRESS_ON     | VALUE_BARO_OSR_P_8 | VALUE_BARO_OSR_T_1    },
    {ADDRESS_BARO_ODR_CONFIG, VALUE_BARO_DEEP_DISABLE | VALUE_BARO_ODR_140 | VALUE_BARO_MODE_NORMAL},
};

static const struct CONFIGURATION LSM6DSV32X[] = {
    {}
};

// ERROR HANDLING
enum DISPLAY_CODES {
    BAROMETER_FAIL  = 0b00000001,
    IMU_FAIL        = 0b00000010,
    FLASH_FAIL      = 0b00000100,
    MicroSD_FAIL    = 0b00000011,
    LORA_FAIL       = 0b00000110,
    GNSS_FAIL       = 0b00000111,
    PASS_1          = 0b00001000,
    PASS_2          = 0b00010000,
    PASS_3          = 0b00100000,
    PASS_4          = 0b00011000,
    PASS_5          = 0b00110000,
    PASS_6          = 0b00111000
};

/**
 * @brief   Basic initialization of KESTREL.
 *          Sets PYRO pins LOW.
 *          Enables USB re-flashing.
 *          Initializes status LEDs.
 *          
 * @param   None.
 * @return  None.
 * @note    Function must be run first.
 */
void SYSTEM_INIT(void);

#endif // CONFIG_H