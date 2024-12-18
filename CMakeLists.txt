cmake_minimum_required(VERSION 3.0)
project(PIA_terrain)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

add_executable(
    findpath 
    main.cpp 
    TerrainMap.cpp 
    Path.cpp 
    NavyPath.cpp 
)