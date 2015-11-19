cmake_minimum_required(VERSION 3.3)

project(VisionAssignment2)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(SOURCE_FILES main.cpp Images.cpp Headers/Images.hpp PointDetector.cpp Headers/PointDetector.hpp Utils.cpp Headers/Utils.hpp Histogram.cpp PageDetector.cpp Headers/PageDetector.hpp Transformer.cpp Headers/Transformer.hpp LineDetector.cpp Headers/LineDetector.hpp)
add_executable(VisionAssignment2 ${SOURCE_FILES})
target_link_libraries( VisionAssignment2 ${OpenCV_LIBS} )