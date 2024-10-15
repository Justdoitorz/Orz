cmake_minimum_required(VERSION 3.10)

project(test)

find_package(GTest REQUIRED)

add_definitions(-D_GNU_SOURCE)


enable_testing()

include_directories(../include)

add_executable(util_test util_test.cpp)
target_link_libraries(util_test PRIVATE orz GTest::GTest GTest::Main)
add_test(NAME util COMMAND util_test)

add_executable(algorithm_test algorithm_test.cpp)
target_link_libraries(algorithm_test PRIVATE orz GTest::GTest GTest::Main)
add_test(NAME algorithm COMMAND algorithm_test)

