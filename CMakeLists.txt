cmake_minimum_required(VERSION 3.29)
project(Images_Template__LabProgr_)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(Images_Template__LabProgr_ src/main.cpp
        src/Image.h
        src/PPM_IO.cpp
        src/PPM_IO.h
        src/Kernel.cpp
        src/Kernel.h
        test/test_main.cpp
)

#download and build googletest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)
