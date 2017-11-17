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
#    "DS_2016/Code_fourni/01_Mire/01_Mire.c"
    "DS_2016/Code_fourni/02_subsampling/time_measure.h"
    "DS_2016/Code_fourni/02_subsampling/02_subsampling.c"
)
add_executable(TestOptimisationDeCode ${SOURCE_FILES})