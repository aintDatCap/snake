#ifndef PAUSE_UI_HPP
#define PAUSE_UI_HPP

#include "game/logic.hpp"

#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

namespace Graphics {
typedef enum {
    PAUSE_EXIT_PROGRAM,
    PAUSE_SELECT_LEVEL,
    PAUSE_RESUME,
} PauseUIDecision;

typedef struct {
    PauseUIDecision action;
    Snake::GameDifficulty game_difficulty;
} PauseUIAction;

class PauseUI {
  private:
    WINDOW *window;
    WINDOW *resume_button;
    WINDOW *level_selector_button;
    WINDOW *exit_button;
    PauseUIAction player_selection;

  public:
    PauseUI(uint16_t width, uint16_t height);
    ~PauseUI();
    PauseUIAction wait_for_user_input();
};
} // namespace Graphics
#endif