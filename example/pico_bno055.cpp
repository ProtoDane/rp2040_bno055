#include "pico/stdlib.h"
#include "rp2040_bno055.hpp"

BNO055 imu;

int main() {

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    int i = imu.begin(i2c1, 26, 27);

    if (i == -1) {
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
    } else {
        while(1) {
            gpio_put(PICO_DEFAULT_LED_PIN, 1);
            sleep_ms(1000);
            gpio_put(PICO_DEFAULT_LED_PIN, 0);
            sleep_ms(1000);
        }
    }

}