#include "game/game_manager.hpp"
#include "game/graphics.hpp"
#include "game/logic.hpp"
#include "graphics.hpp"
#include <cassert>
#include <ctime>
#include <cstdlib>
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

SnakeGameManager::~SnakeGameManager() { //destructor
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

void SnakeGameManager::start_game(GameDifficulty game_difficulty, uint16_t level) {
    ListElement<LevelInfo>* elem = this->levels.get_element_at(level-1);
    if(!elem){}

    delete this->menu_ui;
    this->menu_ui = nullptr;

    this->game = new Game(window_height, window_width, game_difficulty);
    this->game_ui = new GameUI(this->game); //rendering a new win for th game...
    //game_ui window settings
    keypad((this->game_ui)->getWindow(), true);  //for arrow keys
    nodelay((this->game_ui)->getWindow(), true); //for non-blocking input

    // TODO: timer, player input
    Direction player_direction = this->get_player_input();
    while (game->update_game(player_direction) == GAME_UNFINISHED && false) {
        game_ui->update_game_window();
        // timer
        usleep(125000); //sleep(in micro-secs) to give time to see frames rendered between each loop
        
        /*#ifdef _WIN32
        // windows timer
        #else
        // linux timer
        #endif*/
    }

    game_ui->update_game_window();
    getch();
    this->next_level();
}

Direction SnakeGameManager::get_player_input() {
    WINDOW* gameWin = (this->game_ui)->getWindow(); 

    int inp = wgetch(gameWin); //inp for game win
    switch(inp == KEY_UP){
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
                this->level_selector_ui = new LevelSelectorUI (this->window_width, this->window_height, 8);
                LevelSelection selected_level = this->level_selector_ui->wait_for_level_input(); //wait till user selects a lvl

                if(selected_level.action == LEVEL_SELECT_PLAY) {
                    this->start_game(player_selection.game_difficulty, selected_level.level);
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

void SnakeGameManager::next_level() {
    // todo
    // if there's no remaining levels, just return;
}
} // namespace Snake