cmake_minimum_required(VERSION 3.20)
project(sum C ASM)

set(CMAKE_EXE_LINKER_FLAGS "-gcc -m64 -mcmodel=medium -Wl,--image-base=0x400000")
add_executable(main main.S sum.S)