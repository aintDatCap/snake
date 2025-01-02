cmake_minimum_required(VERSION 3.20.0)
project(Snake VERSION 0.1.0 LANGUAGES C CXX)


set(SNAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
add_executable(Snake ${SNAKE_SOURCE_DIR}/main.cpp)
