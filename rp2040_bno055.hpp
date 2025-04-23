#pragma once
#include <stdio.h>
#include <cstdint>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define BNO055_I2C_ADDR     0x28
#define BNO055_CHIP_ID_ADDR 0x00

class BNO055 {

    public:

        i2c_inst_t i2c_port;

        int begin(i2c_inst_t *i2c, uint8_t sda, uint8_t scl);

};