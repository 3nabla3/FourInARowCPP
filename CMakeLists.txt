cmake_minimum_required(VERSION 3.22)
project(FourInARowCPP)
set(CMAKE_CXX_STANDARD 23)


if(UNIX)
	set(GLOG_INCLUDE_DIR cmake-build-release/vendor/glog/ vendor/glog/src/)
	set(GLFW_INCLUDE_DIR vendor/glfw/include)
endif(UNIX)
if(WIN32)
	message("Windows config")
	SET(GLOG_INCLUDE_DIR out/build/x64-Debug/vendor/glog/ vendor/glog/src/)
endif(WIN32)


#target_include_directories(FourInARowCPP PRIVATE . src)
#target_include_directories(glog PRIVATE ${GLOG_INCLUDE_DIR})
include_directories(. src ${GLOG_INCLUDE_DIR} ${GLFW_INCLUDE_DIR})


set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
add_subdirectory(vendor/glog)
add_subdirectory(vendor/glfw)

file(GLOB SOURCE_FILES *.cpp *.h ./src/*.cpp ./src/*.h)

add_executable(FourInARowCPP ${SOURCE_FILES})
if(WIN32)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT FourInARowCPP)
endif(WIN32)

target_precompile_headers(FourInARowCPP PUBLIC pch.h)
target_link_libraries(FourInARowCPP glog glfw)
# windows and unix have different names for the opengl library
if(UNIX)
	target_link_libraries(FourInARowCPP GL)
endif(UNIX)
if(WIN32)
	target_link_libraries(FourInARowCPP opengl32)
endif(WIN32)