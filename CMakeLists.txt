cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})


project(async LANGUAGES C CXX
        VERSION ${PROJECT_VESRION}
        DESCRIPTION "homework 9 - async command processing")

find_package(Threads REQUIRED)

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
target_link_libraries(GTest::GTest INTERFACE gtest_main gmock_main ${CMAKE_THREAD_LIBS_INIT})

configure_file(version.h.in version.h)

add_subdirectory(libasync)

add_executable(async main.cpp
        libasync/athread.h)

add_executable(gtest_async gtest_async.cpp)

set_target_properties(async gtest_async PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(libasync
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(async PRIVATE
    libasync ${CMAKE_THREAD_LIBS_INIT}
)

# target_link_options(async PRIVATE -static-libgcc -static-libstdc++)

target_link_libraries(gtest_async
        PRIVATE
        GTest::GTest
        libasync
)


if (MSVC)
    target_compile_options(async PRIVATE
        /W4
    )
    target_compile_options(libasync PRIVATE
        /W4
    )
    target_compile_options(test_version PRIVATE
        /W4
    )
else ()
    target_compile_options(async PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(libasync PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(gtest_async PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()


install(TARGETS async RUNTIME DESTINATION bin)
install(TARGETS libasync RUNTIME DESTINATION lib)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT kovalevdenis@gmail.com)

 set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.31)")

include(CPack)

enable_testing()

add_test(gtest_async gtest_async)
