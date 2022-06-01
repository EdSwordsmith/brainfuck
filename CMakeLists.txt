cmake_minimum_required(VERSION 3.22)
project(brainfuck C)

set(CMAKE_C_STANDARD 17)

add_executable(brainfuck src/visitors.c src/nodes.c src/main.c)
