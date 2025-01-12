#include "game/game_manager.hpp"
#include "game/graphics.hpp"
#include "game/logic.hpp"
#include <ncurses.h>
namespace Snake {

SnakeGameManager::SnakeGameManager(uint16_t window_width, uint16_t window_height, List<LevelInfo> levels) {
    this->levels = levels;
    this->game = nullptr;
    this->game_ui = nullptr;
    this->menu_ui = new MenuUI(window_width, window_height);

    this->window_height = window_height;
    this->window_width = window_width;

    PlayerSelection player_selection = this->menu_ui->wait_for_user_input();
    switch (player_selection.action) {
    case MENU_PLAY_GAME: {
        this->start_game(player_selection.game_difficulty);
        break;
    }
    case MENU_EXIT_PROGRAM: {
        clear();
        return;
    }
    }
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
        case MENU_PLAY_GAME: {
            this->start_game(player_selection.game_difficulty);
            break;
        }
        case MENU_EXIT_PROGRAM: {
            clear();
            return;
        }
        }
    }
}

Direction SnakeGameManager::get_player_input() {
    // TODO
    return DIRECTION_UP;
}

void SnakeGameManager::next_level() {
    // todo
    // if there's no remaining levels, just return;
}
} // namespace Snake