cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(myalloc VERSION ${PROJECT_VESRION})

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

add_executable(myalloc myalloc.cpp main.cpp mycont.h)
add_executable(test_myalloc test_myalloc.cpp myalloc.cpp myalloc.h mycont.h
        )

set_target_properties(myalloc test_myalloc PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(myalloc
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(test_myalloc
        PRIVATE
        GTest::GTest
)

if (MSVC)
    target_compile_options(myalloc PRIVATE
        /W4
    )
    target_compile_options(myalloc PRIVATE
        /W4
    )
    target_compile_options(test_myalloc PRIVATE
        /W4
    )
else ()
    target_compile_options(myalloc PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(myalloc PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_myalloc PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()


install(TARGETS myalloc RUNTIME DESTINATION bin)

enable_testing()

add_test(test_myalloc test_myalloc)
