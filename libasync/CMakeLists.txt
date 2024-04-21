
add_library(libasync SHARED libasync.cpp
        inputproc.h
        icommand.h
        fileoutput.h
        consoleoutput.h
        inputproc.cpp
        fileoutput.cpp
        command.cpp
        consoleoutput.cpp
)

set_property(TARGET libasync PROPERTY OUTPUT_NAME async)

find_package(Threads REQUIRED)

# We need this directory, and users of our library will need it too
target_include_directories(libasync PUBLIC ../include)

# target_compile_features(libasync PRIVATE cxx_std_17)

set_target_properties(libasync PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# We need this directory, and users of our library will need it too
target_include_directories(libasync PUBLIC ../include)
