cmake_minimum_required(VERSION 3.26)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})


project(fashio_mnist LANGUAGES C CXX
        VERSION ${PROJECT_VESRION}
        DESCRIPTION "homework 12 - fashion MNIST model inference")



configure_file(version.h.in version.h)

add_executable(fashio_mnist main.cpp logistic_predictor.cpp helpers.cpp )


target_link_libraries(fashio_mnist
    PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
)
      

set_target_properties( PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)


if (MSVC)
    target_compile_options(fashio_mnist PRIVATE
        /W4
    )
else ()
    target_compile_options(fashio_mnist PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()


install(TARGETS fashio_mnist RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_NAME fashio_mnist)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT kovalevdenis@gmail.com)

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.31)")

include(CPack)

