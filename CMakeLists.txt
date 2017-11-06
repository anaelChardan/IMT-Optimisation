cmake_minimum_required(VERSION 3.8)
project(TestOptimisationDeCode)

set(CMAKE_C_STANDARD 11)

#set(SOURCE_FILES main.c )
#set(SOURCE_FILES base.c)
set(
    SOURCE_FILES
    common/image_utils.c
    common/image_utils.h
    common/bmp.c
    common/bmp.h
    "DS 2016/Code fourni/01 Mire/01_Mire.c"
    "DS 2016/Code fourni/01 Mire/time_measure.h"
)
add_executable(TestOptimisationDeCode ${SOURCE_FILES})