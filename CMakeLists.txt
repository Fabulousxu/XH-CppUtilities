cmake_minimum_required(VERSION 3.28)
project(XH-CppTools)

set(CMAKE_CXX_STANDARD 20)

add_executable(XH-CppTools main.cpp
        function.h
        function_traits.h
        cpp_tools.h
        predicate.h
)
