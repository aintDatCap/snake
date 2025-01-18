#ifndef GRAPHICS_CPP
#define GRAPHICS_CPP

#include "graphics.hpp"
#include "game/logic.hpp"
#include <cstdint>
#include <ncurses.h>

#define PUT_CENTERED_TEXT(window, text)                                                                                \
    mvwprintw(window, getmaxy(window) / 2, (getmaxx(window) - sizeof(text)) / 2, "%s", text)

#define PUT_CENTERED_COLORED_TEXT(window, text, color)                                                                 \
    wattron(window, COLOR_PAIR(color));                                                                                \
    PUT_CENTERED_TEXT(window, text);                                                                                   \
    wattroff(window, COLOR_PAIR(color))

#define IS_INSIDE_WINDOW(window, x, y)                                                                                 \
    (getbegx(window) <= x && (getbegx(window) + getmaxx(window)) >= x) &&                                              \
        (getbegy(window) <= y && (getbegy(window) + getmaxy(window)) >= y)

void start_ncurses() {
    initscr();
    start_color();

    init_pair(Snake::RED_TEXT, COLOR_RED, COLOR_BLACK);
    init_pair(Snake::GREEN_TEXT, COLOR_GREEN, COLOR_BLACK);
    init_pair(Snake::BLUE_TEXT, COLOR_BLUE, COLOR_BLACK);

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
    wattron(this->window, COLOR_PAIR(RED_TEXT));
    Coordinates apple_position = this->game->get_apple_position();
    mvwaddch(this->window, apple_position.y, apple_position.x, 'o');
    wattroff(this->window, COLOR_PAIR(RED_TEXT));

    // Rendering the snake
    wattron(this->window, COLOR_PAIR(GREEN_TEXT));
    Coordinates snake_head = this->game->get_snake_head_position();
    mvwaddch(this->window, snake_head.y, snake_head.x, '@');

    for (uint16_t i = 0; i < this->game->get_snake_body()->size(); ++i) {
        Coordinates coord = this->game->get_snake_body()->get_element_at(i)->value;
        mvwaddch(this->window, coord.y, coord.x, '*');
    }
    wattroff(this->window, COLOR_PAIR(GREEN_TEXT));
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
    PUT_CENTERED_TEXT(play_game_button, "Gioca");

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
        PUT_CENTERED_COLORED_TEXT(difficulty_button, "Facile", GREEN_TEXT);
        break;
    case DIFFICULTY_NORMAL:
        PUT_CENTERED_COLORED_TEXT(difficulty_button, "Normale", BLUE_TEXT);
        break;
    case DIFFICULTY_HARD:
        PUT_CENTERED_COLORED_TEXT(difficulty_button, "Difficile", RED_TEXT);
        break;
    }
    wrefresh(this->difficulty_button);
    curs_set(0);
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
                if (mouse_event.bstate & BUTTON1_CLICKED || mouse_event.bstate & BUTTON1_PRESSED) {

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
        } else if (c == KEY_EXIT) { // NOT WORKING AS INTENDED
            player_selection.action = MENU_EXIT_PROGRAM;
            return player_selection;
        }
    }
}

} // namespace Snake

#endif