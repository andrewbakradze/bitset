cmake_minimum_required(VERSION 3.16)
project(bitset LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

add_library(bitset_lib STATIC
    bit_logic.cpp
    bitset.cpp
    bitset-view.cpp
)
target_compile_features(bitset_lib PUBLIC cxx_std_20)
target_include_directories(bitset_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(bitset main.cpp)
target_link_libraries(bitset PRIVATE bitset_lib)
