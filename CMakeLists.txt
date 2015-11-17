cmake_minimum_required(VERSION 3.3)

project(VisionAssignment2)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp Images.cpp Images.hpp PointDetector.cpp PointDetector.h Utils.cpp Utils.h Histogram.cpp Histogram.h)
add_executable(VisionAssignment2 ${SOURCE_FILES})
target_link_libraries( VisionAssignment2 ${OpenCV_LIBS} )