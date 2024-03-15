cmake_minimum_required(VERSION 3.27)
project(ring_buffer)

set(THIS ${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

set(inc_dir include)

set(HEADERS
    ${inc_dir}/ring_buffer.h)

set(SOURCES
    main.cpp)

add_executable(${THIS} ${SOURCES} ${HEADERS})

target_include_directories(${THIS} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
