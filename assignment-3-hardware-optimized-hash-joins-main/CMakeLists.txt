cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(hw3)

include(lib/gtest.cmake)

include_directories(include)

# Set compile flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_library(ohj_lib src/OptimizedHashJoins.cpp)

add_executable(ohj src/main.cpp)
target_link_libraries(ohj ohj_lib)

### Tests
add_executable(test_all test/OptimizedHashJoins.cpp)
target_link_libraries(test_all ohj_lib gtest gtest_main pthread)
