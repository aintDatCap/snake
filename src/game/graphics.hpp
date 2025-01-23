#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "logic.hpp"
#include "utils/list.hpp"
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
    RED_TEXT = 1,
    GREEN_TEXT = 2,
    BLUE_TEXT = 3,
} UIColors;

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
    MENU_SELECT_LEVEL,
} MenuAction;

typedef struct {
    MenuAction action;
    GameDifficulty game_difficulty;
    uint32_t level;
} PlayerSelection;

class MenuUI {
  private:
    WINDOW *window;
    WINDOW *play_game_button;
    WINDOW *difficulty_button;
    WINDOW *exit_button;
    PlayerSelection player_selection;

    void render_difficulty_button();

  public:
    MenuUI(uint16_t width, uint16_t height);
    ~MenuUI();

    PlayerSelection wait_for_user_input();
};

typedef enum {
    LEVEL_SELECT_EXIT,
    LEVEL_SELECT_PLAY
} LevelSelectAction;

typedef struct {
    LevelSelectAction action;
    uint16_t level;
} LevelSelection;

class LevelSelectorUI {
  private:
    uint32_t width;
    uint32_t height;
    List<LevelInfo>* levels;

    WINDOW *window;
    WINDOW **level_buttons;
    LevelSelection level_selection;

  public:
    LevelSelectorUI(uint16_t width, uint16_t height, List<LevelInfo>* levels);
    ~LevelSelectorUI();

    LevelSelection wait_for_level_input();
    void render_level_buttons();
};

} // namespace Snake

#endif