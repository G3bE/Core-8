cmake_minimum_required(VERSION 3.14)
project(Core-8)

set(CMAKE_CXX_STANDARD 11)

# tests without headers btw
add_executable(Core-8 src/main.cpp src/binary.h src/cpu/cpu.cpp src/cpu/cpu.h src/memory/ram.cpp src/memory/ram.h src/assembler/assembler.cpp src/assembler/assembler.h src/assembler/instructions.cpp src/assembler/instructions.h)

add_executable(Raw_CPU tests/raw_cpu.cpp src/cpu/cpu.cpp src/memory/ram.cpp)
add_executable(Byte_code tests/byte_code.cpp src/cpu/cpu.cpp src/memory/ram.cpp)
add_executable(Run_thru tests/run_thru.cpp src/cpu/cpu.cpp src/memory/ram.cpp)
