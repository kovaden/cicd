cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bulk VERSION ${PROJECT_VESRION})

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
target_link_libraries(GTest::GTest INTERFACE gtest_main gmock_main)

configure_file(version.h.in version.h)

add_executable(bulk main.cpp)

add_library(bulklib bulklib.cpp
        output.h
        inputproc.h
        icommand.h
        inputproc.cpp
        fileoutput.h
        fileoutput.cpp
        consoleoutput.h
)

add_executable(gtest_bulk gtest_bulk.cpp)

set_target_properties(bulk bulklib gtest_bulk PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(bulklib
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(bulk PRIVATE
    bulklib
)

# target_link_options(bulk PRIVATE -static-libgcc -static-libstdc++)

target_link_libraries(gtest_bulk
        PRIVATE
        GTest::GTest
        bulklib
)


if (MSVC)
    target_compile_options(bulk PRIVATE
        /W4
    )
    target_compile_options(bulklib PRIVATE
        /W4
    )
    target_compile_options(test_version PRIVATE
        /W4
    )
else ()
    target_compile_options(bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(bulklib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(gtest_bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()


install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT kovalevdenis@gmail.com)

# set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.31)")

include(CPack)

enable_testing()

add_test(gtest_bulk gtest_bulk)
