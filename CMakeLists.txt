cmake_minimum_required(VERSION 3.16)
project(polycompare)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

string(APPEND CMAKE_EXE_LINKER_FLAGS " ")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(APPEND CMAKE_CXX_FLAGS_DEBUG          " -fsanitize=address,undefined,leak")
string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO " -fsanitize=address,undefined,leak")

add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wextra-semi -Wmissing-noreturn -Wconversion -Wsign-conversion)

find_package(benchmark REQUIRED)

add_executable(poly_var bench/poly_var.cpp)
target_include_directories(poly_var PRIVATE include)
target_link_libraries(poly_var PRIVATE benchmark::benchmark)

add_executable(poly_virt bench/poly_virt.cpp)
target_include_directories(poly_virt PRIVATE include)
target_link_libraries(poly_virt PRIVATE benchmark::benchmark)

add_executable(poly_vec bench/poly_vec.cpp)
target_include_directories(poly_vec PRIVATE include)
target_link_libraries(poly_vec PRIVATE benchmark::benchmark)
