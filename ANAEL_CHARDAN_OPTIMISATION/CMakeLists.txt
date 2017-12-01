cmake_minimum_required(VERSION 3.8)
project(ANAEL_CHARDAN_OPTIMISATION)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        02-Decoupage/02_decoupage.c
        02-Decoupage/time_measure.h
        common/time_mesure.c
        common/image_utils.c
        common/image_utils.h
        common/bmp.h
        common/bmp.c
        )
add_executable(ANAEL_CHARDAN_OPTIMISATION ${SOURCE_FILES})