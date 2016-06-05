# Ensure CMake version is at least 3.0
cmake_minimum_required(VERSION 3.0)

# Sets the PROJECT_NAME variable
project(qtcvclgl_interop CXX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

# Find the Qt library
find_package(Qt5Core    REQUIRED)
find_package(Qt5Widgets REQUIRED)

# Find the OpenCV library
find_package(OpenCV 3.0.0 REQUIRED core imgproc videoio)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories("D:/FOUBERT/projets/opensource_workspace/opencv/3rdparty/include/opencl/1.2")

file(GLOB_RECURSE ui_files *.ui)
qt5_wrap_ui(ui_headers ${ui_files})

# file(GLOB_RECURSE hpp_files  *.hpp)
# qt5_wrap_cpp(moc_headers ${hpp_files})

file(GLOB_RECURSE src_files *.cpp *.qrc)

add_executable(${PROJECT_NAME} ${src_files} ${ui_headers})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui ${OpenCV_LIBS})
