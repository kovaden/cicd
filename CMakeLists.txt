cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(mapreduce VERSION ${PROJECT_VESRION})

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

find_package(Threads REQUIRED)

add_executable(mapreduce main.cpp)

add_library(mapreducelib mapreduce.cpp)
add_executable(test_mr test_mr.cpp)

set_target_properties(mapreduce mapreducelib test_mr PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(mapreduce
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(mapreduce PRIVATE
        mapreducelib
        Threads::Threads
)

# target_link_options(helloworld_cli PRIVATE -static-libgcc -static-libstdc++)

target_link_libraries(test_mr
        PRIVATE
        GTest::GTest
        mapreducelib
)

if (MSVC)
    target_compile_options(mapreduce PRIVATE
        /W4
    )
    target_compile_options(mapreducelib PRIVATE
        /W4
    )
    target_compile_options(test_mr PRIVATE
        /W4
    )
else ()
    target_compile_options(mapreduce PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(mapreducelib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_mr PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()


install(TARGETS mapreduce RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT kovalevdenis@gmail.com)

# set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.31)")

include(CPack)

enable_testing()

add_test(test_mr test_mr)
