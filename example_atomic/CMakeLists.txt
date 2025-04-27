cmake_minimum_required(VERSION 3.24)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_bno055_atomic C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

pico_sdk_init()

add_executable(${PROJECT_NAME} 
    pico_bno055_atomic.cpp
)

include(${CMAKE_CURRENT_LIST_DIR}/../rp2040_bno055.cmake)
include(C:/VSARM/lib/RP2040_Pseudo_Atomic/CMakeLists.txt)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} 
    pico_stdlib 
    rp2040_bno055
    pico_multicore
    RP2040Atomic
)