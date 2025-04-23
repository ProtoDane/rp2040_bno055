#include "pico/stdlib.h"
#include "rp2040_bno055.hpp"

BNO055 imu(i2c1, 26, 27);

int main() {

    stdio_init_all();
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

        while(1) {
            int8_t t = imu.getTemp();
            printf("Sensor Temp: %d C", t);
            sleep_ms(1000);
        }
    }

}