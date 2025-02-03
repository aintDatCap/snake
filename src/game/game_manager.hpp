#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "game/level_list.hpp"
#include "game/logic.hpp"
#include "graphics/game_ui.hpp"
#include "graphics/level_selection_ui.hpp"
#include "graphics/menu_ui.hpp"
#include <cstdint>

namespace Snake {
class SnakeGameManager {
  private:
    uint16_t window_width;
    uint16_t window_height;
    LevelList *level_list;
    Game *game;
    Graphics::GameUI *game_ui;
    Graphics::MenuUI *menu_ui;
    Graphics::LevelSelectionUI *level_selector_ui;

  public:
    SnakeGameManager(uint16_t window_width, uint16_t window_height, LevelList *levels);
    ~SnakeGameManager();

    void start_game(GameDifficulty game_difficulty, uint32_t level_id);
    void show_menu();
    uint32_t get_frame_duration(uint32_t level);
    void next_level();
    Direction get_player_input();
};

} // namespace Snake

#endif