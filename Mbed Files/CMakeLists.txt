cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
set(APP_TARGET ebike-antitheft-fw)

include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

add_subdirectory(${MBED_PATH})

add_executable(${APP_TARGET})

# Include-Pfade (Root + Firmware/src)
target_include_directories(${APP_TARGET}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Firmware/src
)

# Deine App-Sourcen
target_sources(${APP_TARGET}
    PRIVATE
        Firmware/src/main.cpp
        Firmware/src/state_machine.cpp
        Firmware/src/motion_mpu6050.cpp
        Firmware/src/gnss_pa1010d.cpp
        Firmware/src/rfid_mfrc522.cpp
        Firmware/src/payload.cpp
        Firmware/src/lora_app.cpp
)

target_link_libraries(${APP_TARGET}
    PRIVATE
        mbed-os
        mbed-lorawan
)

mbed_set_post_build(${APP_TARGET})

