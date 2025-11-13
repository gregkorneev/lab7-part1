cmake_minimum_required(VERSION 3.12)

project(lab7_1 LANGUAGES CXX)

# Стандарт C++ и базовые флаги
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Исполняемый файл и все исходники
add_executable(sorting_searching_project
    src/main.cpp
    src/sorting_algorithms.cpp
    src/searching_algorithms.cpp
    src/closest_pair.cpp
    src/results_csv.cpp
)

target_include_directories(sorting_searching_project PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(sorting_searching_project PRIVATE -Wall -Wextra -Wpedantic)
endif()
