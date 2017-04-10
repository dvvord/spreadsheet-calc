find_package(GTest REQUIRED)
enable_testing()
include_directories(${GTEST_INCLUDE_DIRS})

add_executable( runUnitTests test_all.cpp )
target_link_libraries(runUnitTests gtest gtest_main pthread utils)
add_test( runUnitTests runUnitTests )