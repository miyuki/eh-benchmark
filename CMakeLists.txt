cmake_minimum_required(VERSION 3.0)

set(GENERATOR "${CMAKE_SOURCE_DIR}/gen.py")
set(CALLS_CPP "${CMAKE_BINARY_DIR}/calls.cpp")
set(CLASSES_CPP "${CMAKE_BINARY_DIR}/classes.cpp")
set(HIERARCHY_CPP "${CMAKE_BINARY_DIR}/hierarchy.cpp")
set(CLASSES_DEF "${CMAKE_BINARY_DIR}/classes_tests.def")
set(HIERARCHY_DEF "${CMAKE_BINARY_DIR}/hierarchy_tests.def")

function(add_generated_src ENTITY NUM DEST_FILE)
    add_custom_command(OUTPUT "${DEST_FILE}"
                       COMMAND "${GENERATOR}" ${ENTITY} -c ${NUM} -o "${DEST_FILE}"
                       DEPENDS "${GENERATOR}")
endfunction()

function(add_generated_header ENTITY NUM DEST_FILE)
    add_custom_command(OUTPUT "${DEST_FILE}"
                       COMMAND "${GENERATOR}" ${ENTITY} --vector -c ${NUM} -o "${DEST_FILE}"
                       DEPENDS "${GENERATOR}")
endfunction()

add_generated_src(calls 100 "${CALLS_CPP}")
add_generated_src(classes 50 "${CLASSES_CPP}")
add_generated_src(hierarchy 50 "${HIERARCHY_CPP}")
add_generated_header(classes 50 "${CLASSES_DEF}")
add_generated_header(hierarchy 50 "${HIERARCHY_DEF}")

add_executable(profile_throw src/profile_throw.cpp)
add_executable(profile_unwind src/profile_unwind.cpp "${CALLS_CPP}")
add_executable(profile_search src/profile_search.cpp "${CLASSES_CPP}")
include_directories("${CMAKE_BINARY_DIR}")
add_executable(benchmark src/benchmark.cpp
                         src/test_objc.m
                         "${CALLS_CPP}"
                         "${CLASSES_CPP}"
                         "${HIERARCHY_CPP}")
set_property(SOURCE src/test_objc.m PROPERTY
             COMPILE_FLAGS "-fobjc-exceptions -std=c99")
set_property(SOURCE src/benchmark.cpp PROPERTY
             OBJECT_DEPENDS "${CLASSES_DEF}" "${HIERARCHY_DEF}")
# Dirty hack. This shoud be in CMAKE_CXX_FLAGS, but
# CMake does not handle CMAKE_CXX_FLAGS correctly: it adds same flags to Objective C
# modules, but the Objective C compiler does not understand "-std=c++14" flag.
set_property(SOURCE src/benchmark.cpp PROPERTY
             COMPILE_FLAGS "-std=c++14")
target_link_libraries(benchmark objc papi)
