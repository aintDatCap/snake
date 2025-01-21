#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "game/graphics.hpp"
#include "game/logic.hpp"
#include "utils/list.hpp"

namespace Snake {
typedef struct {

} LevelInfo;

class SnakeGameManager {
  private:
    uint16_t window_width;
    uint16_t window_height;
    Game *game;
    MenuUI *menu_ui;
    GameUI *game_ui;
    LevelSelectorUI *level_selector_ui;
    List<LevelInfo> levels;

  public:
    SnakeGameManager(uint16_t window_width, uint16_t window_height, List<LevelInfo> levels);
    ~SnakeGameManager();
    
    void start_game(GameDifficulty game_difficulty);
    void show_menu();
    void next_level();
    Direction get_player_input();
};

} // namespace Snake

#endif