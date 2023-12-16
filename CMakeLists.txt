cmake_minimum_required(VERSION 3.10)

project(matrix)

add_executable(matrix main.cpp)

set_target_properties(matrix PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(matrix
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_compile_options(matrix PRIVATE
    -Wall -Wextra -pedantic -Werror
)

install(TARGETS matrix RUNTIME DESTINATION bin)
