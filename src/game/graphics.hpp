#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "logic.hpp"
#include <cstdint>
#include <functional>

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
};

class MenuUI {
  private:
    WINDOW *window;
    std::function<void()> start_game_callback;

  public:
    MenuUI(uint16_t width, uint16_t height, std::function<void()> start_game_callback);
    ~MenuUI();
};

} // namespace Snake

#endif