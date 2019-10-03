cmake_minimum_required (VERSION 3.5)

project (scheduling
   VERSION 1.0
   DESCRIPTION "Some scheduling things")

# I gave up and just installed googletest on my system.
include(GoogleTest)
find_package(GTest REQUIRED)

enable_testing()
add_subdirectory(jobshop)
