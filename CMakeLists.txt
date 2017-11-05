cmake_minimum_required(VERSION 3.8)
project(TestOptimisationDeCode)

set(CMAKE_C_STANDARD 11)

#set(SOURCE_FILES main.c )
#set(SOURCE_FILES base.c)
set(SOURCE_FILES base_2.c)
add_executable(TestOptimisationDeCode ${SOURCE_FILES})