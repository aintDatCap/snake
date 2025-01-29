#ifndef LEVEL_SELECTION_UI_HPP
#define LEVEL_SELECTION_UI_HPP

#include "game/logic.hpp"
#include "utils/list.hpp"
#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

namespace Graphics {

typedef enum {
    LEVEL_SELECT_EXIT,
    LEVEL_SELECT_PLAY
} LevelSelectAction;

typedef struct {
    LevelSelectAction action;
    uint32_t level;
} LevelSelection;

class LevelSelectionUI {
  private:
    uint32_t width;
    uint32_t height;
    List<Snake::LevelInfo> *levels;

    WINDOW *window;
    WINDOW **level_buttons;
    LevelSelection level_selection;

  public:
    LevelSelectionUI(uint16_t width, uint16_t height, List<Snake::LevelInfo> *levels);
    ~LevelSelectionUI();

    LevelSelection wait_for_level_input();
    void render_level_buttons();
};
} // namespace Graphics
#endif