cmake_minimum_required(VERSION 3.22)
project(FourInARowTest)
set(CMAKE_CXX_STANDARD 23)

#include_directories(../src/ ../)
file(GLOB SOURCE_FILES *.cpp ../src/*cpp)
add_executable(FourInARowTest ${SOURCE_FILES})
