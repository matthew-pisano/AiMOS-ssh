cmake_minimum_required(VERSION 3.27)
project(aimos C)

set(CMAKE_C_STANDARD 23)

add_executable(aimos src/main.c
        src/argparser.h
        src/argparser.c
        src/commands.c
        src/commands.h)

target_compile_options(aimos PRIVATE -Wall -Wextra)
