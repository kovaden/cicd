cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(graph_edit VERSION ${PROJECT_VESRION})

add_executable(graph_edit main.cpp
        Document.cpp
        Document.h
        model/Primitive.cpp
        model/Primitive.h
        model/Point.cpp
        model/Point.h
        model/Line.cpp
        model/Line.h
        model/Rectangle.cpp
        model/Rectangle.h
        model/Circle.cpp
        model/Circle.h
        controller/Controller.cpp
        controller/Controller.h
        model/Coord.cpp
        model/Coord.h
        error.h
        view/View.cpp
        view/View.h
        model/Model.h)

set_target_properties(graph_edit PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(helloworld_cli PRIVATE
        /W4
    )
    target_compile_options(helloworld PRIVATE
        /W4
    )
    target_compile_options(test_version PRIVATE
        /W4
    )
else ()
    target_compile_options(graph_edit PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(graph_edit PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(graph_edit PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS graph_edit RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT kovalevdenis@gmail.com)

include(CPack)
