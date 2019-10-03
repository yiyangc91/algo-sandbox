cmake_minimum_required (VERSION 3.5)

project (strsearch
   VERSION 1.0
   DESCRIPTION "Some String Searching Stuff")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/vendor")
include_directories(${CMAKE_BINARY_DIR}/vendor/include)
link_directories(${CMAKE_BINARY_DIR}/vendor/lib)

find_package(GTest CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)
find_package(Threads REQUIRED)

# todo:
# - create different implementations of string search
# - compile each search as a static library (bonus: compile both?)
#   - figure out how the shared libraries are linked
# - create a test-only binary that includes googletest and googlebench.
#   - how do you include dependencies in a project cleanly?

set(IMPL_FILES
   impl/base.cpp
   impl/naive.cpp
   impl/reference.cpp
   impl/stacksearch.cpp
   impl/presearch.cpp
   impl/kmp.cpp
   impl/hash.cpp)

enable_testing()
add_executable(runUnitTests
   run_tests.cpp ${IMPL_FILES})
add_executable(runBench
   bench.cpp ${IMPL_FILES}
   generate/testgen.cpp)
target_link_libraries(runBench benchmark ${CMAKE_THREAD_LIBS_INIT})

# Link test executable against gtest & gtest_main
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
   target_link_libraries(runUnitTests gtestd ${CMAKE_THREAD_LIBS_INIT})
else ()
   target_link_libraries(runUnitTests gtest ${CMAKE_THREAD_LIBS_INIT})
endif ()

add_test(
    NAME runUnitTests
    COMMAND runUnitTests
)
