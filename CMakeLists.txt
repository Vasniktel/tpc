cmake_minimum_required(VERSION 3.13)
project(tpc C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(tpc
        src/main.c
        src/tpc.c
        src/scanner.c
        src/parser.c
        src/generator.c)