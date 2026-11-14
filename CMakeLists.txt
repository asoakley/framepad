# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD pico)

include(pico_sdk_import.cmake)
project(Pico-ResTouch-X_X_code)
pico_sdk_init()

add_subdirectory(examples)
add_subdirectory(config)
add_subdirectory(lcd)
add_subdirectory(font)
add_subdirectory(fatfs)
add_subdirectory(sdcard)
add_subdirectory(comms)
add_subdirectory(usb)

include_directories(examples)
include_directories(config)
include_directories(lcd)
include_directories(font)
include_directories(fatfs)
include_directories(sdcard)
include_directories(comms)
include_directories(usb)
include_directories(.)

add_executable(main 
app/main.cpp
app/FramepadApp.cpp
app/DeviceState.cpp
app/UsbCallbacks.cpp
HIDTask.c
LEDTask.c
SleepTask.c
TouchTask.c
LCDTask.c
StreamDeck.c
)

target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/app
)

# enable usb output, disable uart output
pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(main)

target_link_libraries(main PUBLIC 
usb
fatfs 
comms
lcd
sdcard
font 
config
examples
pico_stdlib 
hardware_spi 
tinyusb_device 
tinyusb_board
)
