#ifndef GRAPHICS_CPP
#define GRAPHICS_CPP

#include "graphics.hpp"
#include "game/logic.hpp"
#include <ncurses.h>
#include <vector>

void start_ncurses() {
    initscr();
    start_color();

    init_pair(Snake::APPLE_COLOR, COLOR_RED, COLOR_BLACK);
    init_pair(Snake::SNAKE_COLOR, COLOR_GREEN, COLOR_BLACK);
}

void stop_ncurses() {
    endwin();
}

namespace Snake {
GameUI::GameUI(Game *game) {
    this->game = game;

    GameTable game_table = game->get_game_table();
    // height, width, posy, posx
    this->window = newwin(game_table.height, game_table.width, 0, 0);
    refresh();

    wborder(this->window, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(this->window);
}

GameUI::~GameUI() {
    delwin(this->window);
}

void GameUI::update_game_window() {
    // Rendering the apple
    wattron(this->window, COLOR_PAIR(APPLE_COLOR));
    Coordinates apple_position = this->game->get_apple_position();
    mvwaddch(this->window, apple_position.y, apple_position.x, 'o');
    wattroff(this->window, COLOR_PAIR(APPLE_COLOR));

    // Rendering the snake
    wattron(this->window, COLOR_PAIR(SNAKE_COLOR));
    Coordinates snake_head = this->game->get_snake_head_position();
    mvwaddch(this->window, snake_head.y, snake_head.x, '@');

    std::vector<Coordinates> snake_body = this->game->get_snake_body().to_vector();

    for (Coordinates coord : snake_body) {
        mvwaddch(this->window, coord.y, coord.x, '*');
    }
    wattroff(this->window, COLOR_PAIR(SNAKE_COLOR));
    wrefresh(this->window);
}

MenuUI::MenuUI(uint16_t width, uint16_t height, std::function<void()> start_game_callback) {
    this->window = newwin(height, width, 0, 0);
    this->start_game_callback = start_game_callback;

    refresh();

    wborder(this->window, 0, 0, 0, 0, 0, 0, 0, 0);

    wrefresh(this->window);
}

MenuUI::~MenuUI() {
    delwin(this->window);
}

} // namespace Snake

#endif