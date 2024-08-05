cmake_minimum_required(VERSION 3.28)
project(CppHelper)

set(CMAKE_CXX_STANDARD 20)

add_executable(CppHelper main.cpp
        Promise.h
        function_traits.h
        hCppHelper.h
        hMemfunc.h
        hPredicate.h
        enum_name.h)
