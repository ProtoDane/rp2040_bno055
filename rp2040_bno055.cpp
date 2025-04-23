/*
    Library for retrieving data from the Adafruit BNO-055 sensor via I2C
    on Pimoroni Servo2040 (or other RP2040 boards, e.g. RaspPi Pico).

    Adapted from BNO055-RP2040-Library by Aryan Kapoor
*/

#include "rp2040_bno055.hpp"

int BNO055::begin(i2c_inst_t *i2c, uint8_t sda, uint8_t scl) {

    i2c_init(i2c, 400 * 1000);

    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(sda);

    sleep_ms(5000);

    uint8_t chipID[1];

    i2c_write_blocking(i2c, BNO055_I2C_ADDR, BNO055_CHIP_ID_ADDR, 1, true);
    i2c_read_blocking(i2c, BNO055_I2C_ADDR, chipID, 1, false);

    if (chipID[0] != 0xA0) {
        return -1;
    }

    return 0;
}

