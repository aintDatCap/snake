#include "game/game_manager.hpp"
#include "game/level_list.hpp"
#include "game/logic.hpp"
#include "graphics/leaderboard_ui.hpp"
#include "graphics/menu_ui.hpp"
#include "graphics/pause_ui.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <unistd.h> //for usleep()...

namespace Snake {

SnakeGameManager::SnakeGameManager(uint16_t window_width, uint16_t window_height, LevelList *levels) {
    std::srand(time(NULL));
    this->level_list = levels;
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

void SnakeGameManager::start_game(GameDifficulty game_difficulty, uint32_t level_id) {

    delete this->menu_ui;
    this->menu_ui = nullptr;

    this->game = new Game(window_height, window_width, game_difficulty, level_id); // obj for game logic

    assert(this->level_list->set_current_level(game_difficulty, level_id));

    this->game_ui = new Graphics::GameUI(this->game); // rendering a new win for the game...

    // game_ui window settings
    keypad((this->game_ui)->getWindow(), true);  // for arrow keys
    nodelay((this->game_ui)->getWindow(), true); // for non-blocking input
    mmask_t oldmask;                             // to save the previous mouse events mask...
    mousemask(0, &oldmask);                      // disable mouse for this win

    // in microseconds
    int32_t remaining_time = GAME_DURATION * 1'000'000;
    // in microseconds
    uint32_t frame_duration = this->get_frame_duration(this->level_list->get_current()->info.id);
    do {

        if (remaining_time <= 0) {
            this->game->win_game();

            LevelListElement *current_level = level_list->get_current();
            current_level->info.high_score = std::max(current_level->info.high_score, game->get_score());

            // if there is any remaining level
            if (this->next_level()) {

                delete game;

                this->game = new Game(window_height, window_width, game_difficulty, level_list->get_current()->info.id);
                this->game_ui->wait_for_user_win_screen();

                delete game_ui;
                this->game_ui = new Graphics::GameUI(this->game);

                remaining_time = GAME_DURATION * 1'000'000;
                frame_duration = this->get_frame_duration(this->level_list->get_current()->info.id);

                game_ui->update_game_window(GAME_DURATION);
                keypad((this->game_ui)->getWindow(), true);  // for arrow keys
                nodelay((this->game_ui)->getWindow(), true);
            } else {
                break;
            }
        }

        Direction player_input = this->get_player_input();
        if (player_input == EXIT) {
            Graphics::PauseUI pause_ui(window_width, window_height);

            mousemask(oldmask, NULL);
            Graphics::PauseUIAction pause_menu_selection = pause_ui.wait_for_user_input();
            if (pause_menu_selection.action == Graphics::PAUSE_EXIT_PROGRAM) {

                break;
            } else if (pause_menu_selection.action == Graphics::PAUSE_RESUME) {
                player_input = DIRECTION_NONE;
            }
            clear();
            mousemask(0, &oldmask);
            game_ui->update_game_window(remaining_time / 1'000'000);
            game_ui->render_content();
        }

        if (game->update_game(player_input) != GAME_UNFINISHED) {
            // managing the ending frame
            break;
        }
        game_ui->update_game_window(remaining_time / 1'000'000);
        // timer
        // sleep(in micro-secs) to give time to see frames rendered between each loop
        usleep(frame_duration);
        remaining_time -= frame_duration;
    } while (game->get_game_result() == GAME_UNFINISHED);

    LevelListElement *current_level = level_list->get_current();
    current_level->info.high_score = std::max(current_level->info.high_score, game->get_score());

    level_list->save_as_file(LEVELS_FILE_NAME);

    if(game->get_game_result() == GAME_LOST) {
        game_ui->wait_for_user_loss_screen();
    }

    mousemask(oldmask, NULL); // restore mouse events
}

uint32_t SnakeGameManager::get_frame_duration(uint32_t level) {
    uint32_t speed;
    switch (this->game->get_game_difficulty()) {
        // the game is made harder by making the snake move every
        // unit of time expressed in microseconds
        // the lower the time intervals the harder the game
        case DIFFICULTY_EASY:
            speed = 300000 - (level * 10000); // Lower speed
            break;
        case DIFFICULTY_NORMAL:
            speed = 250000 - (level * 15000); // Moderate speed
            break;
        case DIFFICULTY_HARD:
            speed = 200000 - (level * 17500); // Faster speed
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
        case 'Q':
        case 'q':
            return EXIT;
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
                this->level_selector_ui = new Graphics::LevelSelectionUI(this->window_width, this->window_height,
                                                                         level_list, player_selection.game_difficulty);

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
            case Graphics::MENU_LEADERBOARD: {
                Graphics::LeaderboardUI leaderboard_ui =
                    Graphics::LeaderboardUI(this->window_width, this->window_height, this->level_list);
                leaderboard_ui.wait_for_user_input();
                break;
            }
            case Graphics::MENU_EXIT_PROGRAM: {
                level_list->save_as_file(LEVELS_FILE_NAME);
                clear();
                return;
            }
            default:
                return;
        }
    }
}

/**
 * Returns true and goes to the next level if there are any
 * otherwise it only returns false
 */
bool SnakeGameManager::next_level() {
    LevelListElement *current = level_list->get_current();
    GameDifficulty current_diff = current->info.difficulty;
    uint32_t current_id = current->info.id;
    if (!level_list->set_current_level(current_diff, current_id + 1)) {
        return false;
    }

    return true;
}

} // namespace Snake