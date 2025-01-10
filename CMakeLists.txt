cmake_minimum_required(VERSION 3.25.0)
project(Snake VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SNAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

add_subdirectory(dependencies)

set(PROGRAM_SOURCES
  ${SNAKE_SOURCE_DIR}/utils/queue.hpp
  ${SNAKE_SOURCE_DIR}/utils/list.hpp

  ${SNAKE_SOURCE_DIR}/game/logic.hpp
  ${SNAKE_SOURCE_DIR}/game/logic.cpp
  ${SNAKE_SOURCE_DIR}/game/graphics.hpp
  ${SNAKE_SOURCE_DIR}/game/graphics.cpp
)

add_executable(Snake ${SNAKE_SOURCE_DIR}/main.cpp ${PROGRAM_SOURCES})

target_include_directories(Snake PUBLIC ${SNAKE_SOURCE_DIR})

target_compile_options(Snake PRIVATE -Wall -Wextra -Wpedantic -Werror)


set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

if(${CURSES_FOUND})
  include_directories(${CURSES_INCLUDE_DIRS})
  target_link_libraries(Snake ${CURSES_LIBRARIES})
endif()



if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

    include(CTest)
    enable_testing()
    add_subdirectory(tests)
    
endif()