cmake_minimum_required(VERSION 3.22)
project(FourInARowTest)
set(CMAKE_CXX_STANDARD 23)

set(BOOST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/boost)
message(${BOOST_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/vendor/boost)

file(GLOB SOURCE_FILES *.cpp)
add_executable(FourInARowTest ${SOURCE_FILES})
target_link_libraries(FourInARowTest LibFourInARowCPP)
