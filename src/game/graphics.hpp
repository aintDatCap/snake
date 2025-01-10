#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "logic.hpp"

#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

namespace Snake {

typedef enum {
    APPLE_COLOR = 0,
    SNAKE_COLOR = 1
} SnakeColors;

class UI {
  private:
    WINDOW *window;
    Game *game;

  public:
    UI(Game *game);
    ~UI();

    void update_game_window();
    void close_window();
    Direction get_player_input();
};
} // namespace Snake

#endif