include(GoogleTest)

set (IMPL_FILES
    backtrack.cpp
    bruteforce.cpp)

add_executable(unitTests
    ${IMPL_FILES}
    alloc_test.cpp)
target_link_libraries(unitTests PRIVATE ${GTEST_BOTH_LIBRARIES})
target_include_directories(unitTests PRIVATE ${GTEST_INCLUDE_DIRS})
message(${GTEST_BOTH_LIBRARIES})

add_executable(benchmarks
    ${IMPL_FILES}
    benchmark.cpp)
target_link_libraries(benchmarks PRIVATE ${BENCHMARK_LIBRARIES})
target_include_directories(benchmarks PRIVATE ${BENCHMARK_INCLUDE_DIRS})

gtest_add_tests(TARGET unitTests)

add_custom_target(benchmark_run
    COMMAND benchmarks
    DEPENDS benchmarks
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
