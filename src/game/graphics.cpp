#ifndef GRAPHICS_CPP
#define GRAPHICS_CPP

#include "graphics.hpp"
#include "game/logic.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
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

#define IS_INSIDE_SUBPAD(subpad, x, y, current_line)                                                                   \
    (getparx(subpad) <= x && (getparx(subpad) + getmaxx(subpad)) >= x) &&                                              \
        ((getpary(subpad) - current_line) <= y && (getpary(subpad) + getmaxy(subpad) - current_line) >= y)

WINDOW *new_bordered_window(uint16_t height, uint16_t width, uint16_t y, uint16_t x) {
    WINDOW *window = newwin(height, width, y, x);
    refresh();

    box(window, 0, 0);
    wrefresh(window);
    return window;
}

void start_ncurses() {
    initscr();
    start_color();

    init_pair(Snake::RED_TEXT, COLOR_RED, COLOR_BLACK);
    init_pair(Snake::GREEN_TEXT, COLOR_GREEN, COLOR_BLACK);
    init_pair(Snake::BLUE_TEXT, COLOR_BLUE, COLOR_BLACK);

    mousemask(ALL_MOUSE_EVENTS, NULL); //for mouse events...

    curs_set(0); 
}

void stop_ncurses() {
    erase();
    endwin();
}

namespace Snake {
    // -GameUI definitions
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

WINDOW* GameUI::getWindow(){
    return this->window;
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

// -MenuIU definitions
MenuUI::MenuUI(uint16_t width, uint16_t height) {
    this->player_selection.game_difficulty = DIFFICULTY_NORMAL;

    this->window = new_bordered_window(height, width, 0, 0);

    this->play_game_button = new_bordered_window(height / 6, width / 4, height - height / 5, (width - width / 4) / 2);

    PUT_CENTERED_TEXT(play_game_button, "Gioca");
    wrefresh(this->play_game_button); //render the playgame option

    this->exit_button = new_bordered_window(height / 6, width / 4, height - height / 2, (width - width / 4) / 2);
    PUT_CENTERED_TEXT(exit_button, "Esci");
    wrefresh(this->exit_button);

    refresh();

    this->difficulty_button = nullptr;
    this->render_difficulty_button();
}

void MenuUI::render_difficulty_button() {
    if (!this->difficulty_button) {
        uint16_t width = getmaxx(this->window);
        uint16_t height = getmaxy(this->window);
        this->difficulty_button = new_bordered_window(height / 6, width / 4, height / 3, (width - width / 4) / 2);
    }

    werase(this->difficulty_button);
    box(this->difficulty_button, 0, 0); // border

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
    delwin(this->difficulty_button);
    delwin(this->exit_button);
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
                        // Instead of directly starting the game, go to the level selector
                        player_selection.action = MENU_SELECT_LEVEL;
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
                    } else if (IS_INSIDE_WINDOW(exit_button, mouse_event.x, mouse_event.y)) {
                        player_selection.action = MENU_EXIT_PROGRAM;
                        return player_selection;
                    }
                }
            }
        } else if (c == KEY_EXIT) { // NOT WORKING AS INTENDED
            player_selection.action = MENU_EXIT_PROGRAM;
            return player_selection;
        }
    }
}

// -LevelSelctorUI definitions
LevelSelectorUI::LevelSelectorUI(uint16_t width, uint16_t height, List<LevelInfo> *levels) {
    erase();
    this->width = width;
    this->height = height;

    this->levels = levels;

    this->window = newpad((levels->get_element_count() + 2) * height / 6, width);

    box(this->window, 0, 0);
    scrollok(this->window, true);
    prefresh(this->window, 0, 0, 0, 0, height - 1, width - 1);

    render_level_buttons();
}

void LevelSelectorUI::render_level_buttons() {
    const size_t level_count = levels->get_element_count();

    this->level_buttons = (WINDOW **)malloc(sizeof(WINDOW *) * level_count);

    for (uint32_t i = 0; i < level_count; ++i) {
        // Coordinates
        uint16_t x = (width - width / 3) / 2;
        uint16_t y = ((i + 1) * height) / 6;
        uint16_t btn_width = width / 3;
        uint16_t btn_height = height / 8;

        level_buttons[i] = subpad(this->window, btn_height, btn_width, y, x);
        box(level_buttons[i], 0, 0);

        char level_text[10];
        snprintf(level_text, sizeof(level_text), "Level %d", i + 1);
        PUT_CENTERED_TEXT(level_buttons[i], level_text);
    }
    prefresh(this->window, 0, 0, 0, 0, height - 1, width - 1);
}

LevelSelection LevelSelectorUI::wait_for_level_input() {
    keypad(this->window, TRUE);

    uint32_t current_line = 0;

    while (true) {
        int c = wgetch(this->window);
        if (c == KEY_MOUSE) {
            MEVENT mouse_event;
            if (getmouse(&mouse_event) == OK) {
                // left button clicked
                if (mouse_event.bstate & BUTTON1_CLICKED || mouse_event.bstate & BUTTON1_PRESSED) {
                    for (uint32_t i = 0; i < levels->get_element_count(); ++i) {
                        if (IS_INSIDE_SUBPAD(level_buttons[i], mouse_event.x, mouse_event.y, (int32_t)current_line)) {
                            this->level_selection.action = LEVEL_SELECT_PLAY;
                            this->level_selection.level = i;
                            return this->level_selection; // Save the value of the selected level (1,2, etc.)
                        }
                    }
                } else if (mouse_event.bstate & BUTTON4_PRESSED) {

                    current_line -= std::min<uint32_t>(2, current_line);
                    prefresh(this->window, current_line, 0, 0, 0, height - 1, width - 1);

                } else if (mouse_event.bstate & BUTTON5_PRESSED) {
                    // avoid thinking too hard on why it works
                    current_line += std::min<uint32_t>(2, (uint32_t)getmaxy(this->window) - (current_line + height));
                    prefresh(this->window, current_line, 0, 0, 0, height - 1, width - 1);
                }
            }
        }
    }
}

// Destructor
LevelSelectorUI::~LevelSelectorUI() {
    for (uint32_t i = 0; i < levels->get_element_count(); ++i) {
        delwin(this->level_buttons[i]);
    }
    delwin(this->window);
    delete this->level_buttons;
    refresh();
}

} // namespace Snake

#endif