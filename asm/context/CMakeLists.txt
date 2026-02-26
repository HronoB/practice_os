cmake_minimum_required(VERSION 3.10)
project(HronoS)

set(CMAKE_C_STANDARD 11)

# Add the executable
add_executable(main
        main.c
        context.S
)

# Add compile options
target_compile_options(main PRIVATE -g -m64 -mcmodel=medium)

# Add link options
target_link_options(main PRIVATE -Wl,--image-base=0x400000)