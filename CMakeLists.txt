cmake_minimum_required(VERSION 3.22)
project(FourInARowCPP)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(vendor/glog)

include_directories(. src cmake-build-release/vendor/glog/ vendor/glog/src/)

add_executable(FourInARowCPP src/main.cpp src/Board.cpp src/Game.cpp src/Game.h)
target_precompile_headers(FourInARowCPP PRIVATE pch.h)
target_link_libraries(FourInARowCPP glog)