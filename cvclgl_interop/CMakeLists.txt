
cmake_minimum_required(VERSION 2.8.8)

project(cvclgl_interop CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB proj_srcs *.cpp)

add_executable(${PROJECT_NAME} ${proj_srcs})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
