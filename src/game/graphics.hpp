#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "logic.hpp"

#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

namespace Snake {
class UI {
  private:
    WINDOW *window;
    Game* game;

  public:
    UI(Game *game) {
        this->game = game;
    }

    void update_game_window();
};
} // namespace Snake

#endif