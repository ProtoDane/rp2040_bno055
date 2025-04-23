/*
    Library for retrieving data from the Adafruit BNO-055 sensor via I2C
    on Pimoroni Servo2040 (or other RP2040 boards, e.g. RaspPi Pico).

    Adapted from BNO055-RP2040-Library by Aryan Kapoor
*/

#include "rp2040_bno055.hpp"

BNO055::BNO055(i2c_inst_t *i2c, uint8_t sda, uint8_t scl) {

    _i2c_port = i2c;
    _sda = sda;
    _scl = scl;

}

bool BNO055::begin(bno055_opmode_t mode) {
    
    gpio_set_function(_sda, GPIO_FUNC_I2C);
    gpio_set_function(_scl, GPIO_FUNC_I2C);
    gpio_pull_up(_sda);
    gpio_pull_up(_sda);

    i2c_init(_i2c_port, 400 * 1000);

    sleep_ms(1000);

    uint8_t chipID[1];

    write(BNO055_CHIP_ID_ADDR);
    read(chipID, 1);

    if (chipID[0] != BNO055_ID) {
        return false;
    }

    setMode(OPERATION_MODE_CONFIG);

    write(BNO055_SYS_TRIGGER_ADDR, 0x20);
    sleep_ms(30);
    while (true) {
        uint8_t buf[1];
        write(BNO055_CHIP_ID_ADDR);
        read(buf, 1);
        
        if (buf[0] == BNO055_ID) {
            break;
        }

        sleep_ms(10);
    }
    sleep_ms(50);

    write(BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);
    sleep_ms(10);

    write(BNO055_PAGE_ID_ADDR, 0);

    write(BNO055_SYS_TRIGGER_ADDR, 0x0);
    sleep_ms(10);

    setMode(mode);
    sleep_ms(20);

    return true;
}

void BNO055::setMode(bno055_opmode_t mode) {
    _mode = mode;
    write(BNO055_OPR_MODE_ADDR, _mode);
    sleep_ms(30);
}

bno055_opmode_t BNO055::getMode() {
    uint8_t buf[1];
    write(BNO055_OPR_MODE_ADDR);
    read(buf, 1);
    return (bno055_opmode_t)buf[0];
}

void BNO055::setExtCrystalUse(bool usextal) {

    bno055_opmode_t modeback = _mode;

    setMode(OPERATION_MODE_CONFIG);
    sleep_ms(25);
    write(BNO055_PAGE_ID_ADDR, 0);
    if (usextal) {
        write(BNO055_SYS_TRIGGER_ADDR, 0x80);
    } else {
        write(BNO055_SYS_TRIGGER_ADDR, 0x00);
    }
    sleep_ms(10);
    setMode(modeback);
    sleep_ms(20);
}

void BNO055::write(bno055_reg_t reg) {
    i2c_write_blocking(_i2c_port, BNO055_I2C_ADDR, (const uint8_t*)&reg, 1, true);
}

void BNO055::write(bno055_reg_t reg, uint8_t value) {
    uint8_t buffer[2] = {(uint8_t) reg, value};
    i2c_write_blocking(_i2c_port, BNO055_I2C_ADDR, buffer, 2, true);
}

void BNO055::read(uint8_t *buffer, int len) {
    i2c_read_blocking(_i2c_port, BNO055_I2C_ADDR, buffer, len, false);
}
