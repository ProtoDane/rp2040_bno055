// Alternate example to test BNO-055 implementation with RP2040's multicore feature.
// The purpose is to isolate time-sensitive functions from the IMU sensor processing. 

#include "pico/stdlib.h"
#include <string.h>
#include "rp2040_bno055.hpp"
#include "pico/multicore.h"

BNO055 imu(i2c1, 26, 27);

void fifo_push_double(double value) {
    uint32_t parts[2];
    memcpy(parts, &value, sizeof(double));
    multicore_fifo_push_blocking(parts[0]);
    multicore_fifo_push_blocking(parts[1]);
}

double fifo_pop_double() {
    uint32_t parts[2];
    parts[0] = multicore_fifo_pop_blocking();
    parts[1] = multicore_fifo_pop_blocking();
    
    double value;
    memcpy(&value, parts, sizeof(double));
    return value;
}

void core1_main() {
    bool i = imu.begin();

    if (!i) {
        multicore_fifo_push_blocking(0x0F);
        while(1) sleep_ms(10);
    }

    uint8_t status, self_test, error;
    imu.getSystemStatus(&status, &self_test, &error);

    multicore_fifo_push_blocking(error);

    if (error != 0) {
        while(1) sleep_ms(10);
    }
    
    vector v;
    while(1) {
        
        if (multicore_fifo_rvalid()) {
            multicore_fifo_drain();
            fifo_push_double(v.x);
            fifo_push_double(v.y);
            fifo_push_double(v.z);
        }
        imu.getVector(VECTOR_EULER, &v);
        sleep_ms(20);
    }
}

int main() {
    stdio_init_all();

    while(!stdio_usb_connected()) sleep_ms(10);

    printf("Launching multicore BNO-055 example...\n");

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    multicore_launch_core1(core1_main);

    printf("Waiting for IMU initialization...\n");

    uint32_t success = multicore_fifo_pop_blocking();

    if (success != 0) {
        printf("An error occurred (CODE: %d)\n", success);
        while(1) sleep_ms(10);
    }

    printf("IMU initialized.  Beginning core0 loop...\n");

    while(1) {
        sleep_ms(500);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(500);
        gpio_put(PICO_DEFAULT_LED_PIN, 1);

        multicore_fifo_push_blocking(0x01);
        vector v;
        v.x = fifo_pop_double();
        v.y = fifo_pop_double();
        v.z = fifo_pop_double();

        printf("Euler | YAW: %.2lf PITCH: %.2lf ROLL: %.2lf\n", v.x, v.y, v.z);
    }

}