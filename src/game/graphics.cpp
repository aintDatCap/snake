#ifndef GRAPHICS_CPP
#define GRAPHICS_CPP

#include "graphics.hpp"
#include "game/logic.hpp"
#include <cstdint>
#include <ncurses.h>
#include <vector>

#define PUT_TEXT_IN_THE_MIDDLE_OF(window, text)                                                                        \
    mvwprintw(window, getmaxy(window) / 2, (getmaxx(this->window) - sizeof(text) - 1) / 2, "%s", text)

void start_ncurses() {
    initscr();
    start_color();

    init_pair(Snake::APPLE_COLOR, COLOR_RED, COLOR_BLACK);
    init_pair(Snake::SNAKE_COLOR, COLOR_GREEN, COLOR_BLACK);

    mousemask(ALL_MOUSE_EVENTS, NULL);
}

void stop_ncurses() {
    erase();
    endwin();
}

namespace Snake {
GameUI::GameUI(Game *game) {
    this->game = game;

    GameTable game_table = game->get_game_table();
    // height, width, posy, posx
    this->window = newwin(game_table.height, game_table.width, 0, 0);
    refresh();

    box(this->window, 0, 0);
    wrefresh(this->window);
}

GameUI::~GameUI() {
    delwin(this->window);
    refresh();
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

    std::vector<Coordinates> snake_body = this->game->get_snake_body()->to_vector();

    for (Coordinates coord : snake_body) {
        mvwaddch(this->window, coord.y, coord.x, '*');
    }
    wattroff(this->window, COLOR_PAIR(SNAKE_COLOR));
    wrefresh(this->window);
    refresh();
}

MenuUI::MenuUI(uint16_t width, uint16_t height) {
    this->window = newwin(height, width, 0, 0);
    refresh();

    box(this->window, 0, 0);

    wrefresh(this->window);

    this->button_window = newwin(height / 6, width / 4, height - height / 5, (width - width / 4) / 2);
    refresh();

    box(this->button_window, 0, 0);
    wrefresh(this->button_window);

    PUT_TEXT_IN_THE_MIDDLE_OF(button_window, "Gioca");

    wrefresh(this->button_window);
    refresh();
}

MenuUI::~MenuUI() {
    delwin(this->window);
    delwin(this->button_window);
    refresh();
}

MenuAction MenuUI::wait_for_user_input() {
    keypad(this->button_window, TRUE);

    while (true) {
        int c = wgetch(this->button_window);
        if (c == KEY_MOUSE) {
            MEVENT mouse_event;
            if (getmouse(&mouse_event) == OK) {
                // left button clicked
                if (mouse_event.bstate & BUTTON1_CLICKED) {
                    return MENU_PLAY_GAME;
                }
            }
        }
    }
}

} // namespace Snake

#endif