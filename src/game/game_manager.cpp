#include "game/game_manager.hpp"
#include "game/graphics.hpp"
#include "game/logic.hpp"
#include "graphics.hpp"
#include <ctime>
#include <cstdlib>

namespace Snake {

SnakeGameManager::SnakeGameManager(uint16_t window_width, uint16_t window_height, List<LevelInfo> levels) {
    std::srand(time(NULL));
    this->levels = levels;
    this->game = nullptr;
    this->game_ui = nullptr;
    this->menu_ui = new MenuUI(window_width, window_height);

    this->window_height = window_height;
    this->window_width = window_width;
    this->show_menu();

}

SnakeGameManager::~SnakeGameManager() {
    if (this->game) {
        delete this->game;
    }

    if (this->game_ui) {
        delete this->game_ui;
    }

    if (this->menu_ui) {
        delete this->menu_ui;
    }
}

void SnakeGameManager::start_game(GameDifficulty game_difficulty) {

    delete this->menu_ui;
    this->menu_ui = nullptr;

    this->game = new Game(window_height, window_width, game_difficulty);
    this->game_ui = new GameUI(this->game);

    // TODO: timer, player input
    Direction player_direction = this->get_player_input();
    while (game->update_game(player_direction) == GAME_UNFINISHED && false) {
        game_ui->update_game_window();
// timer
#ifdef _WIN32
// windows timer
#else
// linux timer
#endif
    }

    game_ui->update_game_window();
    getch();

    this->next_level();
}

void SnakeGameManager::show_menu() {
    while (true) {
        delete this->game;
        this->game = nullptr;
        delete this->game_ui;
        this->game_ui = nullptr;

        this->menu_ui = new MenuUI(window_width, window_height);

        PlayerSelection player_selection = this->menu_ui->wait_for_user_input();
        switch (player_selection.action) {
        case MENU_SELECT_LEVEL: {
            this->level_selector_ui = new LevelSelectorUI (this->window_width, this->window_height);
            LevelSelection selected_level = this->level_selector_ui->wait_for_level_input();

            if(selected_level.action == LEVEL_SELECT_PLAY) {
                 this->start_game(player_selection.game_difficulty);
            } else if(selected_level.action == LEVEL_SELECT_EXIT) {
                break;
            }
            break;
        }
        case MENU_EXIT_PROGRAM: {
            clear();
            return;
        }
        }
    }
}
/*
LevelSelectorUI level_selector(getmaxx(this->window), getmaxy(this->window));
                        LevelSelectAction selected_level = level_selector.wait_for_level_input(); // Save the value of the selected level (1,2, etc.)
                        player_selection.level = static_cast<uint32_t>(selected_level);
*/
Direction SnakeGameManager::get_player_input() {
    // TODO
    return DIRECTION_UP;
}

void SnakeGameManager::next_level() {
    // todo
    // if there's no remaining levels, just return;
}
} // namespace Snake