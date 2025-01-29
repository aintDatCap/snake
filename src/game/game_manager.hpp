#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "game/logic.hpp"
#include "graphics/menu_ui.hpp"
#include "graphics/game_ui.hpp"
#include "graphics/level_selection_ui.hpp"
#include "utils/list.hpp"

namespace Snake {
class SnakeGameManager {
  private:
    uint16_t window_width;
    uint16_t window_height;
    Game *game;
    Graphics::MenuUI *menu_ui;
    Graphics::GameUI *game_ui;
    Graphics::LevelSelectionUI *level_selector_ui;
    List<LevelInfo> levels;

  public:
    SnakeGameManager(uint16_t window_width, uint16_t window_height, List<LevelInfo> levels);
    ~SnakeGameManager();

    void start_game(GameDifficulty game_difficulty, uint32_t level);
    void show_menu();
    LevelInfo *next_level();
    Direction get_player_input();
};

} // namespace Snake

#endif