aux_source_directory(. DIR_USB_SRCS)

include_directories(../config)

add_library(usb ${DIR_USB_SRCS})
target_link_libraries(usb PUBLIC config)
