aux_source_directory(. DIR_COMMS_SRCS)

include_directories(../config)

add_library(comms ${DIR_COMMS_SRCS})
target_link_libraries(comms PUBLIC config)
