cmake_minimum_required(VERSION 3.22)
project(LibFourInARowCPP)
set(CMAKE_CXX_STANDARD 23)

SET(GLOG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vendor/glog/src/ ${CMAKE_BINARY_DIR}/vendor/glog/)
SET(GLAD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vendor/glad/include)
set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vendor/glfw/include)

include_directories(${GLOG_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})

file(GLOB SOURCE_FILES *.cpp *.h)

add_library(LibFourInARowCPP STATIC ${SOURCE_FILES})
target_link_libraries(LibFourInARowCPP glog glfw glad)
target_precompile_headers(LibFourInARowCPP PUBLIC pch.h)
