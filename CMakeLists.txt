cmake_minimum_required(VERSION 3.21)
project(ConvexVolume)

set(CMAKE_CXX_STANDARD 14)

include_directories(extern)
include_directories(include)

add_executable(ConvexVolume main.cpp VolumeSimilarity.cpp VolumeSimilarity.h)
