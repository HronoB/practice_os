cmake_minimum_required(VERSION 3.20)
project(syscall_exercises C)

# Явно указываем, что это не Windows
set(CMAKE_SYSTEM_NAME Linux)

# Отключаем стандартную библиотеку
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdlib -static -no-pie -m64")

add_executable(syscall_program
        entry.S
        syscall.c
        main.c
)

target_compile_options(syscall_program PRIVATE -Wall -Wextra -g)
target_link_options(syscall_program PRIVATE -Wl,--image-base=0x400000)