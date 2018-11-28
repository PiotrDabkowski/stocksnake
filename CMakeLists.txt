cmake_minimum_required(VERSION 3.12)
project(_stocksnake)

set(CMAKE_CXX_STANDARD  11)

file(GLOB_RECURSE ssources stockfish/**.cpp stockfish/**.h)
include_directories(stockfish)

add_subdirectory(pybind11)
pybind11_add_module(_stocksnake wrapper.cpp  ${ssources})

