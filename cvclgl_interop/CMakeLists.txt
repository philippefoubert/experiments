
cmake_minimum_required(VERSION 2.8.8)

project(exple_umat_opengl CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB proj_srcs *.cpp)

add_executable(exple_umat_opengl ${proj_srcs})
target_link_libraries(exple_umat_opengl ${OpenCV_LIBS})
