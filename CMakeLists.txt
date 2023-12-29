cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(helloworld VERSION ${PROJECT_VESRION})

find_package(Boost COMPONENTS program_options filesystem regex unit_test_framework REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 17)

configure_file(version.h.in version.h)

add_executable(bayan main.cpp
        bayan/BayanFile.cpp
        bayan/BayanFile.h
        bayan/MatchGroup.cpp
        bayan/MatchGroup.h
        bayan/BayanSearcher.cpp
        bayan/BayanSearcher.h
        bayan/FileHash.cpp
        bayan/FileHash.h
        bayan/HashFactory.cpp
        bayan/HashFactory.h
        bayan/md5hash.cpp
        bayan/md5hash.h
        bayan/IFile.h
        bayan/FileOpener.h
        bayan/ProgramOptions.cpp
        bayan/ProgramOptions.h
)

add_executable(test_bayan test_bayan.cpp
        bayan/BayanFile.cpp
        bayan/BayanFile.h
        bayan/FileHash.cpp
        bayan/FileHash.h
        bayan/HashFactory.cpp
        bayan/HashFactory.h
        bayan/md5hash.cpp
        bayan/md5hash.h
        bayan/ProgramOptions.cpp
        bayan/ProgramOptions.h)

target_link_libraries(bayan
        ${Boost_LIBRARIES}
)

target_link_libraries(test_bayan
    ${Boost_LIBRARIES}
)

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
    target_compile_options(test_bayan PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT kovalevdenis@gmail.com)

# set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.31)")

include(CPack)

enable_testing()

add_test(test_bayan test_bayan)
