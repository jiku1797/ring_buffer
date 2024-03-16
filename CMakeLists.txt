cmake_minimum_required(VERSION 3.27)
project(ring_buffer)

set(THIS ${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

set(inc_dir include)
set(test_dir tests)

set(HEADERS
    ${inc_dir}/ring_buffer.h
    ${inc_dir}/ring_buffer_iterator.h
)

set(SOURCES
    ${test_dir}/test_ring_buffer.cpp

    main.cpp
)

add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/googletest)

enable_testing()

add_executable(${THIS} ${SOURCES} ${HEADERS})

target_link_libraries(${THIS} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${THIS})

target_include_directories(${THIS} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest/googletest/include/gtest
)
