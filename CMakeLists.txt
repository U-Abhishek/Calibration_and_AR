cmake_minimum_required(VERSION 3.5.0)
project(calibration_and_ar VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(AugmentedReality src/augmentedReality.cpp src/objects_n_filesave.cpp) # ${HEADER_FILES})
add_executable(calibration src/calibration.cpp src/objects_n_filesave.cpp) # ${HEADER_FILES})

target_link_libraries(AugmentedReality ${OpenCV_LIBS})
target_include_directories(AugmentedReality PRIVATE include)
target_link_libraries(calibration ${OpenCV_LIBS})
target_include_directories(calibration PRIVATE include)