cmake_minimum_required(VERSION 3.13)

project(vision)
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(floor_detection floor_detection.cpp) 
target_link_libraries( floor_detection ${OpenCV_LIBS} )
