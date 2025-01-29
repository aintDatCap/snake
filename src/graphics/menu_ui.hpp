#ifndef MENU_UI_HPP
#define MENU_UI_HPP

#include "game/logic.hpp"

#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

namespace Graphics {
typedef enum {
    MENU_EXIT_PROGRAM,
    MENU_SELECT_LEVEL,
} MenuUIDecision;

typedef struct {
    MenuUIDecision action;
    Snake::GameDifficulty game_difficulty;
} MenuUIAction;

class MenuUI {
  private:
    WINDOW *window;
    WINDOW *play_game_button;
    WINDOW *difficulty_button;
    WINDOW *exit_button;
    MenuUIAction player_selection;

    void render_difficulty_button();

  public:
    MenuUI(uint16_t width, uint16_t height);
    ~MenuUI();

    MenuUIAction wait_for_user_input();
};
} // namespace Graphics
#endif