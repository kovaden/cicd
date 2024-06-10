cmake_minimum_required(VERSION 3.26)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})


project(join_server LANGUAGES C CXX
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

configure_file(version.h.in version.h)

add_executable(join_server main.cpp server.cpp schema.cpp command.cpp parser.cpp executor.cpp)

# target_include_directories(join_server
#     PRIVATE
#     ${Boost_INCLUDE_DIR}
#     ${CMAKE_CURRENT_BINARY_DIR}
#     libasync
# )

target_link_libraries(join_server
    PRIVATE
    ${Boost_LIBRARYDIR}
    Boost::program_options
    ${CMAKE_THREAD_LIBS_INIT}
)
      

set_target_properties( PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)


if (MSVC)
    target_compile_options(join_server PRIVATE
        /W4
    )
else ()
    target_compile_options(join_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()


install(TARGETS join_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_NAME join_server)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT kovalevdenis@gmail.com)

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.31)")

include(CPack)

