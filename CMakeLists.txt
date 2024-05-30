cmake_minimum_required(VERSION 3.26)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})


project(bulk_server LANGUAGES C CXX
        VERSION ${PROJECT_VESRION}
        DESCRIPTION "homework 10 - network command processing")


set(BOOST_INCLUDE_LIBRARIES system program_options)
set(BOOST_ENABLE_CMAKE ON)


# include(FetchContent)
# FetchContent_Declare(
#   Boost
#   GIT_REPOSITORY https://github.com/boostorg/boost.git
#   GIT_PROGRESS TRUE
#   GIT_TAG boost-1.85.0
# )

# FetchContent_MakeAvailable(Boost)

find_package(Threads REQUIRED)
find_package(Boost 1.60 REQUIRED COMPONENTS program_options REQUIRED) 

include_directories( ${Boost_INCLUDE_DIRS} )

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

add_executable(bulk_server main.cpp server.cpp
        libasync/athread.h)

# target_include_directories(bulk_server
#     PRIVATE
#     ${Boost_INCLUDE_DIR}
#     ${CMAKE_CURRENT_BINARY_DIR}
#     libasync
# )

target_link_libraries(bulk_server
    PRIVATE
    ${Boost_LIBRARYDIR}
    Boost::program_options
    libasync ${CMAKE_THREAD_LIBS_INIT}
)
      

add_executable(gtest_async gtest_async.cpp)

set_target_properties(bulk_server gtest_async PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(libasync
    PRIVATE "${CMAKE_BINARY_DIR}"
)


# target_link_options(async PRIVATE -static-libgcc -static-libstdc++)

target_link_libraries(gtest_async
        PRIVATE
        GTest::GTest
        libasync
)


if (MSVC)
    target_compile_options(bulk_server PRIVATE
        /W4
    )
    target_compile_options(libasync PRIVATE
        /W4
    )
    target_compile_options(test_version PRIVATE
        /W4
    )
else ()
    target_compile_options(bulk_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(libasync PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(gtest_async PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()


install(TARGETS bulk_server RUNTIME DESTINATION bin)
install(TARGETS libasync RUNTIME DESTINATION lib)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_NAME bulk_server)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT kovalevdenis@gmail.com)

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.31)")

include(CPack)

enable_testing()

add_test(gtest_async gtest_async)
