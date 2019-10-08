cmake_minimum_required(VERSION 3.1)

project(employees)

# Basic Executable
add_executable(interactive
   interactive.cpp
   employee.cpp)

# Find the threads package
# TODO: what does this do?
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Use ExternalProject to build googletest, do not install
# Just export the interface directly (this is way better than my approach?)
include(ExternalProject)
ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/v1.10.x.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(gtest source_dir binary_dir)

add_library(libgtest INTERFACE)
add_dependencies(libgtest gtest)
target_link_libraries(libgtest
    INTERFACE Threads::Threads
              "${binary_dir}/lib/libgtest_main.a"
              "${binary_dir}/lib/libgtest.a")
target_include_directories(libgtest INTERFACE "${source_dir}/googletest/include")

# Enable the testing (maybe cmake enable test thingo here required?)
enable_testing()
include(GoogleTest)

add_executable(unittests
   gtest.cpp
   employee.cpp)
target_link_libraries(unittests PRIVATE libgtest)
target_include_directories(unittests PRIVATE libgtest)

gtest_add_tests(TARGET unittests)
