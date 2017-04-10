cmake_minimum_required(VERSION 2.8)

project (SpreadSheetCalc)
file(GLOB SpreadSheetCalc_SRC
    "*.h"
    "*.cpp"
)

add_executable(spreadc ${SpreadSheetCalc_SRC})
set (CMAKE_CXX_FLAGS "-Wall -std=c++14 -O0 -DWITHGPERFTOOLS -lprofiler -g")
target_link_libraries(spreadc m pthread)
target_compile_features(spreadc PRIVATE cxx_range_for)
