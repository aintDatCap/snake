#ifndef GRAPHICS_CPP
#define GRAPHICS_CPP

#include "graphics.hpp"
#include "game/logic.hpp"
#include "ncurses/ncurses.h"
#include <vector>

namespace Snake {
UI::UI(Game *game) {
    this->game = game;

    initscr();
    start_color();

    GameTable game_table = game->get_game_table();
    // height, width, posy, posx
    this->window = newwin(game_table.height, game_table.width, 0, 0);
    refresh();

    wborder(this->window, 0, 0, 0,0,0,0,0,0);
    wrefresh(this->window);
}

UI::~UI() {
    delwin(this->window);
    endwin();
}

void UI::update_game_window() {
    Coordinates snake_head = this->game->get_snake_head_position();
    mvwaddch(this->window, snake_head.y, snake_head.x, '@');

    std::vector<Coordinates>  snake_body = this->game->get_snake_body().to_vector();
    
    for(Coordinates coord: snake_body) {
        mvwaddch(this->window, coord.y, coord.x, '*');
    }
    wrefresh(this->window);
}
} // namespace Snake

#endif