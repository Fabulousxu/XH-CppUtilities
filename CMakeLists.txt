cmake_minimum_required(VERSION 3.28)
project(XH-CppTools)

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}  main.cpp )