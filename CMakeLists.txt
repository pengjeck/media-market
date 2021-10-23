cmake_minimum_required(VERSION 3.20)
project(media_market)

set(CMAKE_CXX_STANDARD 14)

# append custom cmake modules to CMAKE_MODULE_PATH before modules take effect
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories(
        /opt/homebrew/Cellar/ffmpeg/4.4_2/include
        ${OpenCV_INCLUDE_DIRS})
link_directories(/opt/homebrew/Cellar/ffmpeg/4.4_2/lib)

find_package(OpenCV REQUIRED)

if (OPENCV_FOUND)
    message(STATUS "opencv library found, include dir ${OpenCV_INCLUDE_DIRS}, link libs ${OpenCV_LIBS}")
endif ()

add_executable(media_market main.cpp)

add_executable(convert_cv_mat
        examples/convert-cv-mat-to-av-frame.cpp)
target_link_libraries(convert_cv_mat
        dl ${OpenCV_LIBS} avformat swscale avutil)
