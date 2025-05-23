# RP2040 BNO-055 Library
I'm working with the Adafruit BNO-055 9DOF IMU sensor for a project that runs on the Pico-SDK in C/C++, but Adafruit's driver is made for the Arduino environment.  So I'm building a custom library that is adapted from two open-source libraries (see Attribution) to fit my project needs.  This should be compatible with the Raspberry Pi Pico and other boards based on the RP2040 microcontroller.

# Usage
You must have the C/C++ Pico-SDK installed on your machine.

In your project's CMakeLists file, add the following lines:
```
include(${CMAKE_CURRENT_LIST_DIR}/../rp2040_bno055.cmake)
target_link_libraries(${PROJECT_NAME} rp2040_bno055)
```
In your source file, add this line to the top of the code to include the library contents to your project:
```
#include rp2040_bno055.hpp
```
Refer to "example" folders for sample implementations for RP2040 projects.

# Attribution
[Adafruit BNO055 Arduino Library]: Primary reference used to make the library that can be used in the Pico-SDK environment for C/C++

[RP2040 BNO055 Library by JRDPropulsion]: Old repo that I referenced to adapt the RP2040-specific functions and compare with the Adafruit library

[Adafruit BNO055 Arduino Library]:https://github.com/adafruit/Adafruit_BNO055/tree/master
[RP2040 BNO055 Library by JRDPropulsion]:https://github.com/JRDPropulsion/BNO055-RP2040-Library/tree/main

Adafruit invests time and resources providing this open source code. Please support Adafruit and open-source hardware by purchasing products from Adafruit!