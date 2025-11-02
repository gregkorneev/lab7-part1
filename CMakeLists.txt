cmake_minimum_required(VERSION 3.16)
project(lab7_part1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lab7lib
    src/sort.cpp
    src/search.cpp
    src/closest_points.cpp
)
target_include_directories(lab7lib PUBLIC include)

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE lab7lib)
