cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD pico_w)

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

add_executable(main 
main.c
)

# enable usb output, disable uart output
pico_enable_stdio_usb(main 0)
pico_enable_stdio_uart(main 1)

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
