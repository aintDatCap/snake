#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "logic.hpp"
#include <cstdint>

#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

void start_ncurses();
void stop_ncurses();

namespace Snake {

typedef enum {
    APPLE_COLOR = 1,
    SNAKE_COLOR = 2
} SnakeColors;

class GameUI {
  private:
    WINDOW *window;
    Game *game;

  public:
    GameUI(Game *game);
    ~GameUI();

    void update_game_window();
    void close_window();
    Direction get_player_input();
};

class MenuUI {
  private:
    WINDOW *window;

  public:
    MenuUI(uint16_t width, uint16_t height);
    ~MenuUI();
};

} // namespace Snake

#endif