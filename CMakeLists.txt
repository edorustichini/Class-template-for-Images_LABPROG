cmake_minimum_required(VERSION 3.29)
project(Images_Template__LabProgr_)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(test)

set(SOURCE_FILES src/Image.h src/Kernel.h src/Kernel.cpp src/PPM_IO.h src/PPM_IO.cpp)
add_executable(Images_Template__LabProgr_ src/main.cpp)
add_library(core ${SOURCE_FILES})
target_link_libraries(Images_Template__LabProgr_ core)

