cmake_minimum_required(VERSION 3.8)
project(TestOptimisationDeCode)

set(CMAKE_C_STANDARD 11)

#set(SOURCE_FILES main.c )
#set(SOURCE_FILES base.c)
set(SOURCE_FILES common/image_utils.c common/image_utils.h common/bmp.c common/bmp.h common/08_time_mesure.c common/08_time_mesure.h 08_Lumiere_contraste.c)
add_executable(TestOptimisationDeCode ${SOURCE_FILES})