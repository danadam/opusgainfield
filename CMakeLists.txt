cmake_minimum_required(VERSION 3.10)
project(opusgainfield)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost 1.60.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_compile_options(-Wall -Wextra -pedantic -Werror=switch)

add_executable(opusgainfield main.cpp options.cpp)
