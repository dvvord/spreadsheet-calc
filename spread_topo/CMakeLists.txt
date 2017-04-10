cmake_minimum_required(VERSION 2.8)

project (SpreadSheetCalc)
file(GLOB SpreadSheetCalc_SRC
    "*.h"
    "*.cpp"
)

file(GLOB Utils_SRC
    "Utils/*.h"
    "Utils/*.cpp"
    "graph.h"
    "graph.cpp"
)

file(GLOB Utils_H_SRC
    "Utils/*.h"
)

add_library(utils STATIC ${Utils_SRC})

add_executable(spreadc ${SpreadSheetCalc_SRC} ${Utils_H_SRC})
target_compile_features(spreadc PRIVATE cxx_range_for)
target_link_libraries(spreadc utils)
set (CMAKE_CXX_FLAGS "-Wall -std=c++14 -O0 -g")

ADD_SUBDIRECTORY (Tests)