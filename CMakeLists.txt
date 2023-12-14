cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(matrix VERSION ${PROJECT_VESRION})

# We do not want gtest files to appear in target debian package
set(INSTALL_GTEST OFF)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.1
)

FetchContent_MakeAvailable(googletest)


add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

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

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT kovalevdenis@gmail.com)

# set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.31)")

include(CPack)

enable_testing()

add_test(test_matrix test_matrix)
