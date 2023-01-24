# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_examples_SRCS 变量
aux_source_directory(. DIR_EXAMPLES_SRCS)

include_directories(../config)
include_directories(../lcd)
include_directories(../font)
include_directories(../fatfs)
include_directories(../sdcard)
include_directories(../comms)

# 生成链接库
add_library(examples ${DIR_EXAMPLES_SRCS})
target_link_libraries(examples PUBLIC config lcd font fatfs sdcard comms)
