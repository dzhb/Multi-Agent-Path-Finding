cmake_minimum_required(VERSION 3.5)
project(SATPlanTest)

set(CMAKE_CXX_STANDARD 14)

add_executable(SATPlanTest main.cpp Astar.h Astar.cpp inc/Agent.h src/tools.cpp inc/tools.h src/MultiAgentSearch.cpp inc/MultiAgentSearch.h)

# Find opencv.
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(SATPlanTest LINK_PRIVATE ${OpenCV_LIBS} libz3.so)
