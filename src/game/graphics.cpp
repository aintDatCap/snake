#ifndef GRAPHICS_CPP
#define GRAPHICS_CPP

#include "graphics.hpp"
#include "game/logic.hpp"
#include <cstdint>
#include <ncurses.h>
#include <vector>

#define PUT_TEXT_IN_THE_MIDDLE_OF(window, text)                                                                        \
    mvwprintw(window, getmaxy(window) / 2, (getmaxx(window) - sizeof(text) - 1) / 2, "%s", text)

#define IS_INSIDE_WINDOW(window, x, y)                                                                                 \
    (getbegx(window) <= x && (getbegx(window) + getmaxx(window)) >= x) &&                                              \
        (getbegy(window) <= y && (getbegy(window) + getmaxy(window)) >= y)

void start_ncurses() {
    initscr();
    start_color();

    init_pair(Snake::APPLE_COLOR, COLOR_RED, COLOR_BLACK);
    init_pair(Snake::SNAKE_COLOR, COLOR_GREEN, COLOR_BLACK);
    mousemask(ALL_MOUSE_EVENTS, NULL);

    curs_set(0);
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
    this->player_selection.game_difficulty = DIFFICULTY_NORMAL;

    this->window = newwin(height, width, 0, 0);
    refresh();

    box(this->window, 0, 0);

    wrefresh(this->window);

    this->play_game_button = newwin(height / 6, width / 4, height - height / 5, (width - width / 4) / 2);
    refresh();

    box(this->play_game_button, 0, 0);
    PUT_TEXT_IN_THE_MIDDLE_OF(play_game_button, "Gioca");

    wrefresh(this->play_game_button);
    refresh();

    this->render_difficulty_button();
}

void MenuUI::render_difficulty_button() {
    if (this->difficulty_button) {
        delwin(difficulty_button);
    }
    uint16_t width = getmaxx(this->window);
    uint16_t height = getmaxy(this->window);

    this->difficulty_button = newwin(height / 6, width / 4, height / 3, (width - width / 4) / 2);
    refresh();

    box(this->difficulty_button, 0, 0);
    switch (this->player_selection.game_difficulty) {
    case DIFFICULTY_EASY:
        PUT_TEXT_IN_THE_MIDDLE_OF(this->difficulty_button, "Facile");
        break;
    case DIFFICULTY_NORMAL:
        PUT_TEXT_IN_THE_MIDDLE_OF(this->difficulty_button, "Normale");
        break;
    case DIFFICULTY_HARD:
        PUT_TEXT_IN_THE_MIDDLE_OF(this->difficulty_button, "Difficile");
        break;
    }
        wrefresh(this->difficulty_button);
}

MenuUI::~MenuUI() {
    delwin(this->window);
    delwin(this->play_game_button);
    refresh();
}

PlayerSelection MenuUI::wait_for_user_input() {

    keypad(this->window, TRUE);

    while (true) {
        int c = wgetch(this->window);
        if (c == KEY_MOUSE) {
            MEVENT mouse_event;
            if (getmouse(&mouse_event) == OK) {
                // left button clicked
                if (mouse_event.bstate & BUTTON1_CLICKED) {

                    if (IS_INSIDE_WINDOW(play_game_button, mouse_event.x, mouse_event.y)) {
                        player_selection.action = MENU_PLAY_GAME;
                        return player_selection;

                    } else if (IS_INSIDE_WINDOW(difficulty_button, mouse_event.x, mouse_event.y)) {

                        switch (player_selection.game_difficulty) {
                        case DIFFICULTY_EASY:
                            player_selection.game_difficulty = DIFFICULTY_NORMAL;
                            break;
                        case DIFFICULTY_NORMAL:
                            player_selection.game_difficulty = DIFFICULTY_HARD;
                            break;
                        case DIFFICULTY_HARD:
                            player_selection.game_difficulty = DIFFICULTY_EASY;
                            break;
                        }
                        render_difficulty_button();
                    }
                }
            }
        } else if(c == KEY_EXIT) { // NOT WORKING AS INTENDED
            player_selection.action = MENU_EXIT_PROGRAM;
            return player_selection;
        }
    }
}

} // namespace Snake

#endif