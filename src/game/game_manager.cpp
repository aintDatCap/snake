#include "game/game_manager.hpp"
#include "game/graphics.hpp"
#include "game/logic.hpp"
namespace Snake {

SnakeGameManager::SnakeGameManager(uint16_t window_width, uint16_t window_height, List<LevelInfo> levels) {
    this->levels = levels;
    this->game_ui = nullptr;
    this->menu_ui = new MenuUI(window_width, window_height, [this]() { this->start_game(); });
}

void SnakeGameManager::start_game() {
    
    delete this->menu_ui;
    this->game = new Game(window_height, window_width);
    this->game_ui = new GameUI(this->game);
    
    //TODO: timer, player input
    Direction player_direction = this->get_player_input();
    while (game->update_game(player_direction) != GAME_UNFINISHED) {
        game_ui->update_game_window();
        // timer
        #ifdef _WIN32
        // windows timer
        #else
        // linux timer
        #endif
    }

    this->next_level();
}

void SnakeGameManager::stop_game() {
    delete this->game;
    delete this->game_ui;
    this->menu_ui = new MenuUI(window_width, window_height, [this]() { this->start_game(); });

}

void SnakeGameManager::next_level() {
    // todo
}
} // namespace Snake