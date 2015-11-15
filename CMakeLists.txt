cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "-g")
set(UNWIND_CPP "${CMAKE_BINARY_DIR}/unwind.cpp")
add_custom_command(OUTPUT "${UNWIND_CPP}"
                   COMMAND "${CMAKE_SOURCE_DIR}/gen.py" calls -c 100 -o "${UNWIND_CPP}")
add_executable(profile_throw src/profile_throw.cpp)
add_executable(profile_unwind src/profile_unwind.cpp "${UNWIND_CPP}")
add_executable(benchmark src/benchmark.cpp
                         src/test_objc.m)
set_property(SOURCE src/test_objc.m PROPERTY
             COMPILE_FLAGS "-fobjc-exceptions -std=c99")
target_link_libraries(benchmark objc papi)
