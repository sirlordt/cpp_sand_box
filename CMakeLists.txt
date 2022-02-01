cmake_minimum_required(VERSION 3.14)

project(main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main
  test01.hpp
  main.cpp
)

target_link_libraries(main PRIVATE rt dl m pthread)
