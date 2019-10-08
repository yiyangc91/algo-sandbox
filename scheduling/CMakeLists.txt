cmake_minimum_required (VERSION 3.5)

project (scheduling
   VERSION 1.0
   DESCRIPTION "Some scheduling things")

include(ExternalProject)

ExternalProject_Add(googletest
   GIT_REPOSITORY https://github.com/google/googletest.git
   CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/vendor/googletest
   -DCMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}/vendor/googletest
   -DCMAKE_BUILD_TYPE=Release
   -DBUILD_SHARED_LIBS=ON
   UPDATE_COMMAND "")

ExternalProject_Add(benchmark
   GIT_REPOSITORY https://github.com/google/benchmark.git
   CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/vendor/benchmark
   -DCMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}/vendor/benchmark
   -DCMAKE_BUILD_TYPE=Release
   -DBENCHMARK_ENABLE_TESTING=OFF
   -DBUILD_SHARED_LIBS=ON
   UPDATE_COMMAND "")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/vendor/googletest")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/vendor/benchmark")

set(BENCHMARK_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/vendor/benchmark/include)
set(BENCHMARK_LIBRARIES ${CMAKE_BINARY_DIR}/vendor/benchmark/lib/${CMAKE_SHARED_LIBRARY_PREFIX}benchmark${CMAKE_SHARED_LIBRARY_SUFFIX})
set(GTEST_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/vendor/googletest/include)
set(GTEST_MAIN_LIBRARY ${CMAKE_BINARY_DIR}/vendor/googletest/lib/${CMAKE_SHARED_LIBRARY_PREFIX}gtest${CMAKE_SHARED_LIBRARY_SUFFIX})
set(GTEST_BOTH_LIBRARIES ${GTEST_MAIN_LIBRARY} ${CMAKE_BINARY_DIR}/vendor/googletest/lib/${CMAKE_SHARED_LIBRARY_PREFIX}gtest_main${CMAKE_SHARED_LIBRARY_SUFFIX})

enable_testing()
add_subdirectory(jobshop)
