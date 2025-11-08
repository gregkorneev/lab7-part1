cmake_minimum_required(VERSION 3.10)
project(SortingSearchingProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включение директорий
include_directories(include)

# Создание исполняемого файла
add_executable(sorting_searching_project
    src/main.cpp
    src/sorting_algorithms.cpp
    src/searching_algorithms.cpp
    src/closest_pair.cpp
)

# Настройки компиляции
if(MSVC)
    target_compile_options(sorting_searching_project PRIVATE /W4)
else()
    target_compile_options(sorting_searching_project PRIVATE -Wall -Wextra -pedantic)
endif()
