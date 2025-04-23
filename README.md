# RP2040 BNO-055 Library
I'm working with the BNO-055 9DOF IMU sensor for a project that runs on the Pico-SDK in C/C++, but wasn't able to find a ready-to-run driver.  So I'm building one that's adapted from the Adafruit BNO055 driver for Arduino core and another Arduino core project for the Raspberry Pi Pico by Aryan Kapoor.  This should be compatible with the Raspberry Pi Pico and other boards based on the RP2040 microcontroller.

# Usage
You must have the C/C++ Pico-SDK installed on your machine.

In your project's CMakeLists file, add the following line:

In your source file, add this line to the top of the code to include the library contents to your project:



# Attribution