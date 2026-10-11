#ifndef BMP585_H
#define BMP585_H

#include "config.h"

struct BARO_OUTPUT {
    uint64_t    time_us;        
    float       pressure_Pa;    
    uint16_t    temperature_cK; 
    bool        error;          
    uint8_t     checksum;
};

#endif // BMP585_H