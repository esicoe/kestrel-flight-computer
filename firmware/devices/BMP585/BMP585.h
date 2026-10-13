#ifndef BMP585_H
#define BMP585_H

#include "config.h"
#include "pins.h"

#include "i2c1.h"
#include "led.h"

#define ADDRESS_BARO_I2C                0x46U   //
#define ADDRESS_BARO_CHIP_ID            0x01U   // R
#define ADDRESS_BARO_REV_ID             0x02U   // R
#define ADDRESS_BARO_CHIP_STATUS        0x11U   // R
#define ADDRESS_BARO_DRIVE_CONFIG       0x13U   // R/W
#define ADDRESS_BARO_INT_CONFIG         0x14U   // R/W
#define ADDRESS_BARO_INT_SOURCE         0x15U   // R/W
#define ADDRESS_BARO_FIFO_CONFIG        0x16U   // R/W
#define ADDRESS_BARO_FIFO_COUNT         0x17U   // R
#define ADDRESS_BARO_FIFO_SEL           0x18U   // R/W
#define ADDRESS_BARO_TEMP_DATA_XLSB     0x1DU   // R
#define ADDRESS_BARO_TEMP_DATA_LSB      0x1EU   // R
#define ADDRESS_BARO_TEMP_DATA_MSB      0x1FU   // R
#define ADDRESS_BARO_PRESS_DATA_XLSB    0x20U   // R
#define ADDRESS_BARO_PRESS_DATA_LSB     0x21U   // R
#define ADDRESS_BARO_PRESS_DATA_MSB     0x22U   // R
#define ADDRESS_BARO_INT_STATUS         0x27U   // R
#define ADDRESS_BARO_STATUS             0x28U   // R
#define ADDRESS_BARO_FIFO_DATA          0x29U   // R
#define ADDRESS_BARO_NVM_ADDR           0x2BU   // R/W
#define ADDRESS_BARO_NVM_DATA_LSB       0x2CU   // R/W
#define ADDRESS_BARO_NVM_DATA_MSB       0x2DU   // R/W
#define ADDRESS_BARO_DSP_CONFIG         0x30U   // R/W
#define ADDRESS_BARO_DSP_IIR            0x31U   // R/W
#define ADDRESS_BARO_OOR_THR_P_LSB      0x32U   // R/W
#define ADDRESS_BARO_OOR_THR_P_MSB      0x33U   // R/W
#define ADDRESS_BARO_OOR_RANGE          0x34U   // R/W
#define ADDRESS_BARO_OOR_CONFIG         0x35U   // R/W
#define ADDRESS_BARO_OSR_CONFIG         0x36U   // R/W
#define ADDRESS_BARO_ODR_CONFIG         0x37U   // R/W
#define ADDRESS_BARO_OSR_EFF            0x38U   // R
#define ADDRESS_BARO_CMD                0x7EU   // R/W
#define VALUE_BARO_CHIP_ID              0x51U   // Chip 0x51
#define VALUE_BARO_STATUS_RDY           0x02U   // Chip ready
#define VALUE_BARO_CMD_SOFT_RESET       0xB6U   // Command reset
#define VALUE_BARO_ODR_IS_VALID         0x80U   // ODR valid
#define VALUE_BARO_PRESS_OFF            0x00U   // Pressure off
#define VALUE_BARO_PRESS_ON             0x40U   // Pressure on
#define VALUE_BARO_OSR_P_1              0x00U   // Oversampling x1
#define VALUE_BARO_OSR_P_2              0x08U   // Oversampling x2
#define VALUE_BARO_OSR_P_4              0x10U   // Oversampling x4
#define VALUE_BARO_OSR_P_8              0x18U   // Oversampling x8
#define VALUE_BARO_OSR_P_16             0x20U   // Oversampling x16
#define VALUE_BARO_OSR_P_32             0x28U   // Oversampling x32
#define VALUE_BARO_OSR_P_64             0x30U   // Oversampling x64
#define VALUE_BARO_OSR_P_128            0x38U   // Oversampling x128
#define VALUE_BARO_OSR_T_1              0x00U   // Oversampling x1
#define VALUE_BARO_OSR_T_2              0x01U   // Oversampling x2
#define VALUE_BARO_OSR_T_4              0x02U   // Oversampling x4
#define VALUE_BARO_OSR_T_8              0x03U   // Oversampling x8
#define VALUE_BARO_OSR_T_16             0x04U   // Oversampling x16
#define VALUE_BARO_OSR_T_32             0x05U   // Oversampling x32
#define VALUE_BARO_OSR_T_64             0x06U   // Oversampling x64
#define VALUE_BARO_OSR_T_128            0x07U   // Oversampling x128
#define VALUE_BARO_IIR_P_0              0x00U   // Coefficient bypass
#define VALUE_BARO_IIR_P_1              0x08U   // Coefficient 1
#define VALUE_BARO_IIR_P_3              0x10U   // Coefficient 3
#define VALUE_BARO_IIR_P_7              0x18U   // Coefficient 7
#define VALUE_BARO_IIR_P_15             0x20U   // Coefficient 15
#define VALUE_BARO_IIR_P_31             0x28U   // Coefficient 31
#define VALUE_BARO_IIR_P_63             0x30U   // Coefficient 63
#define VALUE_BARO_IIR_P_127            0x38U   // Coefficient 127
#define VALUE_BARO_IIR_T_0              0x00U   // Coefficient bypass
#define VALUE_BARO_IIR_T_1              0x01U   // Coefficient 1
#define VALUE_BARO_IIR_T_3              0x02U   // Coefficient 3
#define VALUE_BARO_IIR_T_7              0x03U   // Coefficient 7
#define VALUE_BARO_IIR_T_15             0x04U   // Coefficient 15
#define VALUE_BARO_IIR_T_31             0x05U   // Coefficient 31
#define VALUE_BARO_IIR_T_63             0x06U   // Coefficient 63
#define VALUE_BARO_IIR_T_127            0x07U   // Coefficient 127
#define VALUE_BARO_DSP_RESERVED         0x03U   // Reserved 0x03
#define VALUE_BARO_DSP_IIR_FLUSH        0x04U   // Flush forced
#define VALUE_BARO_DSP_SHDW_IIR_T       0x08U   // Filtered temperature
#define VALUE_BARO_DSP_FIFO_IIR_T       0x10U   // Filtered FIFO-temperature
#define VALUE_BARO_DSP_SHDW_IIR_P       0x20U   // Filtered pressure
#define VALUE_BARO_DSP_FIFO_IIR_P       0x40U   // Filtered FIFO-pressure
#define VALUE_BARO_DSP_OOR_IIR_P        0x80U   // Filtered out-of-range
#define VALUE_BARO_INT_DRDY             0x01U   // Interrupt data-ready
#define VALUE_BARO_INT_FIFO_FULL        0x02U   // Interrupt FIFO-full
#define VALUE_BARO_INT_FIFO_THS         0x04U   // Interrupt FIFO-threshold
#define VALUE_BARO_INT_OOR_P            0x08U   // Interrupt out-of-range
#define VALUE_BARO_MODE_STANDBY         0x00U   // Mode standby
#define VALUE_BARO_MODE_NORMAL          0x01U   // Mode normal
#define VALUE_BARO_MODE_FORCED          0x02U   // Mode forced
#define VALUE_BARO_MODE_NONSTOP         0x03U   // Mode non-stop
#define VALUE_BARO_ODR_240              0x00U   // Rate 240Hz
#define VALUE_BARO_ODR_218              0x04U   // Rate 218.537Hz
#define VALUE_BARO_ODR_199              0x08U   // Rate 199.111Hz
#define VALUE_BARO_ODR_179              0x0CU   // Rate 179.2Hz
#define VALUE_BARO_ODR_160              0x10U   // Rate 160Hz
#define VALUE_BARO_ODR_149              0x14U   // Rate 149.333Hz
#define VALUE_BARO_ODR_140              0x18U   // Rate 140Hz
#define VALUE_BARO_ODR_130              0x1CU   // Rate 129.855Hz
#define VALUE_BARO_ODR_120              0x20U   // Rate 120Hz
#define VALUE_BARO_ODR_110              0x24U   // Rate 110.164Hz
#define VALUE_BARO_ODR_100              0x28U   // Rate 100.299Hz
#define VALUE_BARO_ODR_90               0x2CU   // Rate 89.6Hz
#define VALUE_BARO_ODR_80               0x30U   // Rate 80Hz
#define VALUE_BARO_ODR_70               0x34U   // Rate 70Hz
#define VALUE_BARO_ODR_60               0x38U   // Rate 60Hz
#define VALUE_BARO_ODR_50               0x3CU   // Rate 50.056Hz
#define VALUE_BARO_ODR_45               0x40U   // Rate 45.025Hz
#define VALUE_BARO_ODR_40               0x44U   // Rate 40Hz
#define VALUE_BARO_ODR_35               0x48U   // Rate 35Hz
#define VALUE_BARO_ODR_30               0x4CU   // Rate 30Hz
#define VALUE_BARO_ODR_25               0x50U   // Rate 25.005Hz
#define VALUE_BARO_ODR_20               0x54U   // Rate 20Hz
#define VALUE_BARO_ODR_15               0x58U   // Rate 15Hz
#define VALUE_BARO_ODR_10               0x5CU   // Rate 10Hz
#define VALUE_BARO_ODR_5                0x60U   // Rate 5Hz
#define VALUE_BARO_ODR_4                0x64U   // Rate 4Hz
#define VALUE_BARO_ODR_3                0x68U   // Rate 3Hz
#define VALUE_BARO_ODR_2                0x6CU   // Rate 2Hz
#define VALUE_BARO_ODR_1                0x70U   // Rate 1Hz
#define VALUE_BARO_ODR_0_5              0x74U   // Rate 0.5Hz
#define VALUE_BARO_ODR_0_25             0x78U   // Rate 0.25Hz
#define VALUE_BARO_ODR_0_125            0x7CU   // Rate 0.125Hz
#define VALUE_BARO_DEEP_DISABLE         0x80U   // Deep-standby off

typedef struct {
    uint64_t    time_us;
    float       pressure_Pa;
    uint16_t    temperature_K;
    bool        error;
    uint8_t     checksum;
} bmp585_output_t;

typedef struct {
    uint8_t reg;
    uint8_t value;
} register_setting_t;

/**
 * @brief   Checks the chip ID and status, then writes the configuration table.
 * @note    Halts with the BAROMETER_FAIL LED code if any step fails.
 */
void BMP585_Initialize(void);

/**
 * @brief   Reads one temperature and pressure sample.
 * @return  The sample, time-stamped. error is set if the I2C read failed.
 */
bmp585_output_t BMP585_Read(void);

#endif // BMP585_H