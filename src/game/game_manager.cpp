#include "game/game_manager.hpp"
#include "game/logic.hpp"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <unistd.h> //for usleep()...

namespace Snake {

SnakeGameManager::SnakeGameManager(uint16_t window_width, uint16_t window_height, List<LevelInfo> levels) {
    std::srand(time(NULL));
    this->levels = levels;
    this->game = nullptr;
    this->game_ui = nullptr;
    this->menu_ui = new Graphics::MenuUI(window_width, window_height);

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

void SnakeGameManager::start_game(GameDifficulty game_difficulty, uint32_t level) {

    // TODO: level management
    if (level == 0) {
    }

    delete this->menu_ui;
    this->menu_ui = nullptr;

    this->game = new Game(window_height, window_width, game_difficulty, level); // obj for game logic
    this->current_level = level;
    this->game_ui = new Graphics::GameUI(this->game); // rendering a new win for the game...

    // game_ui window settings
    keypad((this->game_ui)->getWindow(), true);  // for arrow keys
    nodelay((this->game_ui)->getWindow(), true); // for non-blocking input
    mmask_t oldmask;                             // to save the previous mouse events mask...
    mousemask(0, &oldmask);                      // disable mouse for this win

#define GAME_DURATION 300 // 300 seconds

    const time_t game_start = time(NULL);
    const uint32_t game_speed = get_frame_duration(this->current_level);
    do {
        time_t elapsed_time = time(NULL) - game_start;
        if (elapsed_time > GAME_DURATION) {
            game->win_game();
            break;
        }

        game_ui->update_game_window(GAME_DURATION - elapsed_time);
        game->update_game(this->get_player_input());
        // timer
        // sleep(in micro-secs) to give time to see frames rendered between each loop
        usleep(game_speed);

    } while (game->get_game_result() == GAME_UNFINISHED);

    mousemask(oldmask, NULL); // restore mouse events
}

uint32_t SnakeGameManager::get_frame_duration(uint32_t level) {
    uint32_t speed;
    switch (this->game->get_game_difficulty()) {
        // the game is made harder by making the snake move every
        // unit of time expressed in microseconds
        // the lower the time intervals the harder the game
        case DIFFICULTY_EASY:                 // tentative values for speed before playtesting
            speed = 300000 - (level * 10000); // Lower speed
            break;
        case DIFFICULTY_NORMAL:
            speed = 250000 - (level * 15000); // Moderate speed
            break;
        case DIFFICULTY_HARD:
            speed = 200000 - (level * 20000); // Faster speed
            break;
        default:
            speed = 125000; // Default speed
            break;
    }

    if (speed < 50000) {
        speed = 50000; // Cap the speed at a minimum interval (e.g., 50 ms)
    }
    return speed;
}

Direction SnakeGameManager::get_player_input() {
    WINDOW *game_win = (this->game_ui)->getWindow();

    int input = wgetch(game_win); // inp for game win

    // TODO: maybe we should get all concurrent inputs,
    // then find out if the player is pressing *only* one arrow,
    // otherwise we're returning DIRECTION_NONE
    while (wgetch(game_win) != ERR) {
        // clearing the input buffer
    }

    switch (input) {
        case 'W':
        case 'w':
        case KEY_UP:
            return DIRECTION_UP;
        case 'S':
        case 's':
        case KEY_DOWN:
            return DIRECTION_DOWN;
        case 'D':
        case 'd':
        case KEY_RIGHT:
            return DIRECTION_RIGHT;
        case 'A':
        case 'a':
        case KEY_LEFT:
            return DIRECTION_LEFT;
        default:
            return DIRECTION_NONE;
    }
}

void SnakeGameManager::show_menu() {
    while (true) {
        delete this->game;
        this->game = nullptr;
        delete this->game_ui;
        this->game_ui = nullptr;

        this->menu_ui = new Graphics::MenuUI(window_width, window_height);

        Graphics::MenuUIAction player_selection = this->menu_ui->wait_for_user_input();

        switch (player_selection.action) {
            case Graphics::MENU_SELECT_LEVEL: {
                this->level_selector_ui =
                    new Graphics::LevelSelectionUI(this->window_width, this->window_height, &levels);

                // Get the selected level
                Graphics::LevelSelection selected_level = this->level_selector_ui->wait_for_level_input();

                delete this->level_selector_ui;
                this->level_selector_ui = nullptr;

                if (selected_level.action == Graphics::LEVEL_SELECT_PLAY) { // Check if level is valid
                    this->start_game(player_selection.game_difficulty, selected_level.level);
                } else {
                    // Handle the case where no valid level was selected, if necessary
                    break;
                }
                break;
            }
            case Graphics::MENU_EXIT_PROGRAM: {
                clear();
                return;
            }
            default:
                return;
        }
    }
}

/**
 * Returns the next level if there are any more levels to play
 * otherwise it returns nullptr
 */

LevelInfo *SnakeGameManager::next_level() {
    // TODO
    return nullptr;
}
} // namespace Snake