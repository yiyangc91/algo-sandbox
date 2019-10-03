
add_executable(unitTests
    backtrack.cpp
    bruteforce.cpp
    alloc_test.cpp)
target_link_libraries(unitTests gtest gtest_main)

gtest_add_tests(TARGET unitTests)
