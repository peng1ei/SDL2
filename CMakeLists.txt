cmake_minimum_required(VERSION 3.15)
project(imooc)

set(CMAKE_CXX_STANDARD 11)

add_executable(imooc main.cpp)
target_link_libraries(imooc SDL2)