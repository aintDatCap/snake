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
};

typedef enum {
    MENU_EXIT_PROGRAM,
    MENU_PLAY_GAME
} MenuAction;

typedef struct {
    MenuAction action;
    GameDifficulty game_difficulty;
} PlayerSelection;

class MenuUI {
  private:
    WINDOW *window;
    WINDOW *play_game_button;
    WINDOW *difficulty_button;
    PlayerSelection player_selection;


  public:
    MenuUI(uint16_t width, uint16_t height);
    ~MenuUI();

    PlayerSelection wait_for_user_input();
    void render_difficulty_button();
};

} // namespace Snake

#endif