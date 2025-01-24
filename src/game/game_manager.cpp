#include "game/game_manager.hpp"
#include "game/graphics.hpp"
#include "game/logic.hpp"
#include "graphics.hpp"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <unistd.h> //for usleep()...

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

SnakeGameManager::~SnakeGameManager() { // destructor
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

void SnakeGameManager::start_game(GameDifficulty game_difficulty, uint16_t level = 0) {

    // TODO: level management
    if (level == 0) {
    }

    delete this->menu_ui;
    this->menu_ui = nullptr;

    this->game = new Game(window_height, window_width, game_difficulty);
    this->game -> set_speed(level);
    this->game_ui = new GameUI(this->game); // rendering a new win for th game...
    // game_ui window settings
    keypad((this->game_ui)->getWindow(), true);  // for arrow keys
    nodelay((this->game_ui)->getWindow(), true); // for non-blocking input

    // TODO: each frame we should:
    // 1) get the player input
    // 2) update the game based on the player input
    // 3) update the game window
    // 4) wait for the next frame

    do {
        game_ui->update_game_window();
        // timer
        // sleep(in micro-secs) to give time to see frames rendered between each loop
        usleep(game -> get_speed());
    } while (game->update_game(this->get_player_input()) == GAME_UNFINISHED);
}

Direction SnakeGameManager::get_player_input() {
    WINDOW *gameWin = (this->game_ui)->getWindow();

    int inp = wgetch(gameWin); // inp for game win

    // TODO: maybe we should get all concurrent inputs,
    // then find out if the player is pressing *only* one arrow,
    // otherwise we're returning DIRECTION_NONE
    switch (inp) {
    case KEY_UP:
        return DIRECTION_UP;
    case KEY_DOWN:
        return DIRECTION_DOWN;
    case KEY_RIGHT:
        return DIRECTION_RIGHT;
    case KEY_LEFT:
        return DIRECTION_LEFT;
    default:
        return DIRECTION_NONE;
        break;
    }
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
                this->level_selector_ui = new LevelSelectorUI(
                    this->window_width, 
                    this->window_height, 
                    &levels);
                LevelSelection selected_level = 
                    this->level_selector_ui->wait_for_level_input();
                
                delete this->level_selector_ui;
                this->level_selector_ui = nullptr;

                if (selected_level.action == LEVEL_SELECT_PLAY) {
                    this->start_game(
                        player_selection.game_difficulty, 
                        selected_level.level
                    );
                } else if (selected_level.action == LEVEL_SELECT_EXIT) {
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

LevelInfo *SnakeGameManager::next_level() {
    // TODO:
    // check if there are any remaining levels
    // if there's no remaining level, just return nullptr;
    // otherwise we return the next level
    return nullptr;
}
} // namespace Snake