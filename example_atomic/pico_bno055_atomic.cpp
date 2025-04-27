// Another implementation of multicore, but I'm using the RP2040-Pseudo-Atomic library
// to optimize the time duration for core0 to retrieve the IMU data.  Previously, I was
// getting times between 100 to 2000 us variation, but with this code, it's down to 5 us.
// Whether that matters is dependent on the use-case, but has been made available regardless.

// RP2040 Pseudo Atomic: https://github.com/TSprech/RP2040_Pseudo_Atomic.git 
// Library is used as-is with no modification.

#include "pico/stdlib.h"
#include <string.h>
#include "rp2040_bno055.hpp"
#include "pico/multicore.h"
#include "RP2040Atomic.hpp"

using namespace patom::types;

BNO055 imu(i2c1, 26, 27);
patomic_uint64_t euler_x;
patomic_uint64_t euler_y;
patomic_uint64_t euler_z;

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
        
        uint64_t x, y, z;
        imu.getVector(VECTOR_EULER, &v);
        memcpy(&x, &v.x, sizeof(double)); // No patomic_double, so copy the binary to uint64_t (same size as double)
        memcpy(&y, &v.y, sizeof(double));
        memcpy(&z, &v.z, sizeof(double));
        euler_x = x;
        euler_y = y;
        euler_z = z;
        sleep_ms(10); // 100 Hz sensor updates
    }
}

int main() {
    stdio_init_all();

    // Required call to claim a spinlock for patom variables
    patom::PseudoAtomicInit();

    while(!stdio_usb_connected()) sleep_ms(10);

    printf("Launching multicore/atomic BNO-055 example...\n");

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    multicore_launch_core1(core1_main);

    printf("Waiting for IMU initialization...\n");

    // Use FIFO to block code until a success return code is received
    uint32_t success = multicore_fifo_pop_blocking();

    if (success != 0) {
        printf("An error occurred (CODE: %d)\n", success);
        while(1) sleep_ms(10);
    }

    printf("IMU initialized.  Beginning core0 loop...\n");

    while(1) {
        // Blink just to provide visual confirmation of the forever loop
        sleep_ms(500);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(500);
        gpio_put(PICO_DEFAULT_LED_PIN, 1);

        uint64_t start = time_us_64();
        vector v;
        uint64_t x, y, z;
        x = euler_x.Load();
        y = euler_y.Load();
        z = euler_z.Load();
        memcpy(&v.x, &x, sizeof(uint64_t)); // Copy binary of uin64_t back to double
        memcpy(&v.y, &y, sizeof(uint64_t));
        memcpy(&v.z, &z, sizeof(uint64_t));
        uint64_t end = time_us_64();
        uint64_t duration = end - start; // Calculate duration of IMU data retrieval process

        printf("Euler | YAW: %.2lf PITCH: %.2lf ROLL: %.2lf | (%d us)\n", v.x, v.y, v.z, duration);
    }

}