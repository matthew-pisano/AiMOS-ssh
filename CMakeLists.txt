cmake_minimum_required(VERSION 3.22)
project(aimos C)

include_directories(include)
set(CMAKE_C_STANDARD 23)

add_executable(aimos src/main.c
        include/argparser.h
        src/argparser.c
        src/commands.c
        include/commands.h)

target_compile_options(aimos PRIVATE -Wall -Wextra)
