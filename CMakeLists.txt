cmake_minimum_required(VERSION 3.22)
project(FourInARowCPP)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(LibFourInARowCPP)
add_subdirectory(vendor/glog)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)
add_subdirectory(tests)

SET(GLOG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vendor/glog/src/ ${CMAKE_BINARY_DIR}/vendor/glog/)
SET(GLAD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vendor/glad/include)
set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vendor/glfw/include)

include_directories(LibFourInARowCPP ${GLOG_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})

file(GLOB SOURCE_FILES src/*cpp) 
add_executable(FourInARowCPP ${SOURCE_FILES})
if(WIN32)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT src/main.cpp)
endif(WIN32)

target_link_libraries(FourInARowCPP LibFourInARowCPP)