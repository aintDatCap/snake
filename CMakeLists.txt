cmake_minimum_required(VERSION 3.25.0)
project(Snake VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SNAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

set(PROGRAM_SOURCES
  ${SNAKE_SOURCE_DIR}/game/snake_body.hpp
  ${SNAKE_SOURCE_DIR}/game/snake_body.cpp

  ${SNAKE_SOURCE_DIR}/game/logic.hpp
  ${SNAKE_SOURCE_DIR}/game/logic.cpp
  ${SNAKE_SOURCE_DIR}/game/level_list.hpp
  ${SNAKE_SOURCE_DIR}/game/level_list.cpp
  ${SNAKE_SOURCE_DIR}/game/game.hpp
  ${SNAKE_SOURCE_DIR}/game/game.cpp
  ${SNAKE_SOURCE_DIR}/game/game_manager.hpp
  ${SNAKE_SOURCE_DIR}/game/game_manager.cpp
  # ${SNAKE_SOURCE_DIR}/game/leaderboard_manager.hpp
  # ${SNAKE_SOURCE_DIR}/game/leaderboard_manager.cpp
  #Graphics
  ${SNAKE_SOURCE_DIR}/graphics/graphics.hpp
  ${SNAKE_SOURCE_DIR}/graphics/graphics.cpp
  ${SNAKE_SOURCE_DIR}/graphics/game_ui.hpp
  ${SNAKE_SOURCE_DIR}/graphics/game_ui.cpp
  ${SNAKE_SOURCE_DIR}/graphics/menu_ui.hpp
  ${SNAKE_SOURCE_DIR}/graphics/menu_ui.cpp
  ${SNAKE_SOURCE_DIR}/graphics/leaderboard_ui.hpp
  ${SNAKE_SOURCE_DIR}/graphics/leaderboard_ui.cpp
  ${SNAKE_SOURCE_DIR}/graphics/level_selection_ui.hpp
  ${SNAKE_SOURCE_DIR}/graphics/level_selection_ui.cpp
  ${SNAKE_SOURCE_DIR}/graphics/pause_ui.hpp
  ${SNAKE_SOURCE_DIR}/graphics/pause_ui.cpp
)

add_executable(Snake ${SNAKE_SOURCE_DIR}/main.cpp ${PROGRAM_SOURCES})

target_include_directories(Snake PUBLIC ${SNAKE_SOURCE_DIR})

target_compile_options(Snake PRIVATE -Wall -Wextra -Wpedantic -Werror)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

if(${CURSES_FOUND})
  include_directories(${CURSES_INCLUDE_DIRS})
  target_link_libraries(Snake PUBLIC ${CURSES_LIBRARIES})
endif()
