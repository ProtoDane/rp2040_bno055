// Example code that utilizes the RP2040-BNO055 library.  Runs on the Raspberry Pi Pico

#include "pico/stdlib.h"
#include "rp2040_bno055.hpp"

BNO055 imu(i2c1, 26, 27);

int main() {

    stdio_init_all();

    while(!stdio_usb_connected()) sleep_ms(10);

    printf("Hello world!\n");

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    bool i = imu.begin();

    if (!i) {
        printf("Something's not right.  Is the IMU wired in properly?\n");
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
    } else {

        printf("IMU initialized successfully!\n");
        gpio_put(PICO_DEFAULT_LED_PIN, 0);

        int8_t t = imu.getTemp();
        printf("Sensor Temp: %d C\n", t);

        uint8_t status, self_test, error;
        imu.getSystemStatus(&status, &self_test, &error);
        printf("System Status: %x\n", status);
        printf("Self-Test Results: %x\n", self_test);
        printf("Error code: %x\n", error);

        while(1) {
            vector v;
            
            // imu.getVector(VECTOR_GYROSCOPE, &v);
            // printf("Gyro | X: %.2lf Y: %.2lf Z: %.2lf\n", v.x, v.y, v.z);
            
            imu.getVector(VECTOR_EULER, &v);
            printf("Euler | YAW: %.2lf PITCH: %.2lf ROLL: %.2lf\n", v.x, v.y, v.z);

            sleep_ms(1000);
        }
    }

}