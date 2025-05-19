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

add_executable(endian_test endian_test.cpp)
target_link_libraries(endian_test PRIVATE orz GTest::GTest GTest::Main)
add_test(NAME endian COMMAND endian_test)

add_executable(tostring_test tostring_test.cpp)
target_link_libraries(tostring_test PRIVATE orz GTest::GTest GTest::Main)
add_test(NAME tostring COMMAND tostring_test)

add_executable(retval_test retval_test.cpp)
target_link_libraries(retval_test PRIVATE orz GTest::GTest GTest::Main)
add_test(NAME retval COMMAND retval_test)

add_executable(date_test date_test.cpp)
target_link_libraries(date_test PRIVATE orz GTest::GTest GTest::Main)
add_test(NAME date COMMAND date_test)

add_executable(alloc_test alloc_test.cpp)
target_link_libraries(alloc_test PRIVATE orz GTest::GTest GTest::Main)
add_test(NAME alloc COMMAND alloc_test)

add_executable(buffer_test buffer_test.cpp)
target_link_libraries(buffer_test PRIVATE orz GTest::GTest GTest::Main)
add_test(NAME buffer COMMAND buffer_test)

add_executable(mutex_test mutex_test.cpp)
target_link_libraries(mutex_test PRIVATE orz GTest::GTest GTest::Main)
add_test(NAME mutex COMMAND mutex_test)

add_executable(bitset_test bitset_test.cpp)
target_link_libraries(bitset_test PRIVATE orz GTest::GTest GTest::Main)
add_test(NAME bitset COMMAND bitset_test)
