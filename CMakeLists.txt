cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_FLAGS "-g")
set(GENERATOR "${CMAKE_SOURCE_DIR}/gen.py")
set(CALLS_CPP "${CMAKE_BINARY_DIR}/calls.cpp")
set(CLASSES_CPP "${CMAKE_BINARY_DIR}/classes.cpp")
add_custom_command(OUTPUT "${CALLS_CPP}"
                   COMMAND "${GENERATOR}" calls -c 100 -o "${CALLS_CPP}"
                   DEPENDS "${GENERATOR}")
add_custom_command(OUTPUT "${CLASSES_CPP}"
                   COMMAND ${GENERATOR} classes -c 50 -o "${CLASSES_CPP}"
                   DEPENDS "${GENERATOR}")
add_executable(profile_throw src/profile_throw.cpp)
add_executable(profile_unwind src/profile_unwind.cpp "${CALLS_CPP}")
add_executable(profile_search src/profile_search.cpp "${CLASSES_CPP}")
add_executable(benchmark src/benchmark.cpp
                         src/test_objc.m
                         "${CALLS_CPP}"
                         "${CLASSES_CPP}")
set_property(SOURCE src/test_objc.m PROPERTY
             COMPILE_FLAGS "-fobjc-exceptions -std=c99")
set_property(SOURCE src/benchmark.cpp PROPERTY
             COMPILE_FLAGS "-std=c++14")
target_link_libraries(benchmark objc papi)
