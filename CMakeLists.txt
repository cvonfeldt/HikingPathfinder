cmake_minimum_required(VERSION 3.16)
project(Project_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project_1
        .idea/.gitignore
        .idea/misc.xml
        .idea/modules.xml
        ".idea/Project 1.iml"
        .idea/workspace.xml
        cmake-build-debug/colorado.dat
        cmake-build-debug/example.dat
        cmake-build-debug/example2.dat
        cmake-build-debug/grand_canyon.dat
        cmake-build-debug/grand_canyon_hires.dat
        cmake-build-debug/kilimanjaro.dat
        cmake-build-debug/kilimanjaro_hires.dat
        lodepng.cpp
        lodepng.h
        main.cpp
        pathfinder.cpp
        pathfinder.h
        picture.cpp
        picture.h)
