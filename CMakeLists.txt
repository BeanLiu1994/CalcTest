cmake_minimum_required(VERSION 2.8)

project(ExpressionTemplate)

add_compile_options(-DRUN_TEST)

add_executable(executable main.cpp)
set_property(TARGET executable PROPERTY CXX_STANDARD 14)