/*
    Library for retrieving data from the Adafruit BNO-055 sensor via I2C
    on Pimoroni Servo2040 (or other RP2040 boards, e.g. RaspPi Pico).

    Adapted from Adafruit BNO055 library for Arduino core and BNO055-RP2040-Library by Aryan Kapoor
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

    if (read(BNO055_CHIP_ID_ADDR) != BNO055_ID) {
        return false;
    }

    setMode(OPERATION_MODE_CONFIG);

    write(BNO055_SYS_TRIGGER_ADDR, 0x20);
    sleep_ms(30);
    while (read(BNO055_CHIP_ID_ADDR) != BNO055_ID) {
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
    return (bno055_opmode_t)read(BNO055_OPR_MODE_ADDR);
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

void BNO055::getSystemStatus(uint8_t *system_status, uint8_t *self_test_result, uint8_t *system_error) {

    write(BNO055_PAGE_ID_ADDR, 0);

    if (system_status != 0) *system_status = read(BNO055_SYS_STAT_ADDR);
    
    if (self_test_result != 0) *self_test_result = read(BNO055_SELFTEST_RESULT_ADDR);

    if (system_error != 0) *system_error = read(BNO055_SYS_ERR_ADDR);

    sleep_ms(200);
}

int8_t BNO055::getTemp() {
    return (int8_t)(read(BNO055_TEMP_ADDR));
}

void BNO055::getVector(vector_type_t vector_type, vector *v) {
    uint8_t buffer[6];
    memset(buffer, 0, 6);
    int16_t x, y, z = 0;

    readBuffer((bno055_reg_t)vector_type, buffer, 6);
    x = ((int16_t)buffer[0]) | (((int16_t)buffer[1]) << 8);
    y = ((int16_t)buffer[2]) | (((int16_t)buffer[3]) << 8);
    z = ((int16_t)buffer[4]) | (((int16_t)buffer[5]) << 8);

    switch (vector_type) {
        
        case VECTOR_MAGNETOMETER:
            v->x = ((double)x) / 16.0;
            v->y = ((double)y) / 16.0;
            v->z = ((double)z) / 16.0;
            break;
        
        case VECTOR_GYROSCOPE:
            v->x = ((double)x) / 16.0;
            v->y = ((double)y) / 16.0;
            v->z = ((double)z) / 16.0;
            break;

        case VECTOR_EULER:
            v->x = ((double)x) / 16.0;
            v->y = ((double)y) / 16.0;
            v->z = ((double)z) / 16.0;
            break;

        case VECTOR_ACCELEROMETER:
            v->x = ((double)x) / 100.0;
            v->y = ((double)y) / 100.0;
            v->z = ((double)z) / 100.0;
            break;

        case VECTOR_LINEARACCEL:
            v->x = ((double)x) / 100.0;
            v->y = ((double)y) / 100.0;
            v->z = ((double)z) / 100.0;
            break;

        case VECTOR_GRAVITY:
            v->x = ((double)x) / 100.0;
            v->y = ((double)y) / 100.0;
            v->z = ((double)z) / 100.0;
            break;
    }
}

//////////////////////////
// Private class functions
//////////////////////////

void BNO055::write(bno055_reg_t reg) {
    i2c_write_blocking(_i2c_port, BNO055_I2C_ADDR, (const uint8_t*)&reg, 1, true);
}

void BNO055::write(bno055_reg_t reg, uint8_t value) {
    uint8_t buffer[2] = {(uint8_t) reg, value};
    i2c_write_blocking(_i2c_port, BNO055_I2C_ADDR, buffer, 2, true);
}

uint8_t BNO055::read(bno055_reg_t reg) {
    uint8_t buf[1];
    write(reg);
    i2c_read_blocking(_i2c_port, BNO055_I2C_ADDR, buf, 1, false);
    return buf[0];
}

void BNO055::readBuffer(bno055_reg_t reg, uint8_t *buffer, int len) {
    write(reg);
    i2c_read_blocking(_i2c_port, BNO055_I2C_ADDR, buffer, len, false);
}
