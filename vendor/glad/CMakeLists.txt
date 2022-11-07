cmake_minimum_required(VERSION 3.22)
project(glad)
set(CMAKE_CXX_STANDARD 23)

include_directories(include/)
add_library(glad STATIC src/glad.c)
