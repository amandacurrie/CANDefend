cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-as)


# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "C:/Users/amand/Pico/CANDefend/CAN_RECEIVER/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(receiver C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()


# Add executable. Default name is the project name, version 0.1
add_executable(receiver
    main.c
    can2040/src/can2040.c
    )

pico_set_program_version(receiver "0.1")

pico_enable_stdio_uart(receiver 1)
pico_enable_stdio_usb(receiver 1)

# Add the standard library to the build
target_link_libraries(receiver
         pico_stdlib)

# Add the standard include files to the build
target_include_directories(receiver PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/can2040/src/
  ${PICO_SDK_PATH}/src/rp2040/
  ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Core/Include/
  ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Device/RaspberryPi/RP2040/Include/
)

# Add any user requested libraries
target_link_libraries(receiver
    hardware_spi
    hardware_pio
    )

pico_add_extra_outputs(receiver)