cmake_minimum_required(VERSION 3.28)
project(CppHelper)

set(CMAKE_CXX_STANDARD 20)

add_executable(CppHelper main.cpp
        promise.h
        function.h
        cpp_helper.h
        predicate.h
        enum_name.h)
