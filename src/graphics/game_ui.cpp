#ifndef GAME_UI_CPP
#define GAME_UI_CPP

#include "graphics/game_ui.hpp"
#include "graphics/graphics.hpp"
#include <algorithm>
#include <cstring>
#include <ncurses.h>

namespace Graphics {

// ASCII Art definitions
const char *ascii_art[24] = {
    "  _________",  " /   _____/",  " \\_____  \\ ", " /        \\", "/_______  /",   " \\      \\/ ",
    " /   |   \\ ", "/    |    \\", "\\_______  /",  "  /  _  \\/ ", " /  /_\\  \\ ", "/    |    \\",
    "\\____|__  /", " ____  __. ",  "|    |/ _| ",   "|      <   ",  "|    |  \\  ",  "|____|__ \\ ",
    "___________",  "\\_   _____/", " |    __)_ ",   " |        \\", "/_______  /",   "        \\/ ",
};
GameUI::GameUI(Snake::Game *game) {
    this->game = game;

    Snake::GameTable game_table = game->get_game_table();

    this->window = newwin(game_table.height, game_table.width, 0, 0);
    refresh();

    Snake::GameTable playable_area = this->game->get_playable_area();

    // Calculate the position of the inner border
    const int start_x = (getmaxx(window) - playable_area.width) / 2;
    const int start_y = (getmaxy(window) - playable_area.height) / 2;

    this->game_window = new_bordered_window(playable_area.height, playable_area.width, start_y, start_x);

    // Draw SNAKE ASCII art on the left side
    draw_art(window, ascii_art, 24, start_y - (24 - playable_area.height) / 2, start_x - 15);

    // Draw SNAKE ASCII art on the right side
    draw_art(window, ascii_art, 24, start_y - (24 - playable_area.height) / 2, start_x + playable_area.width + 3);
}

GameUI::~GameUI() {
    delwin(this->window);
    delwin(this->game_window);
    refresh();
}

WINDOW *GameUI::getWindow() {
    return this->window;
}

void GameUI::update_game_window(int32_t remaining_time) {
    // Clear the window
    // werase(this->window);
    // box(this->window, 0, 0);

    // Rendering the time and score
    wattron(window, A_BOLD | COLOR_PAIR(YELLOW_TEXT));
    mvwprintw(window, 0, 2, "Score: %5u", this->game->get_score());
    mvwprintw(window, 0, getmaxx(window) - 12, "Time: %3d", remaining_time);
    wattroff(window, A_BOLD | COLOR_PAIR(YELLOW_TEXT));

    wrefresh(this->window);

    werase(this->game_window);
    // borders
    wattron(this->game_window, COLOR_PAIR(BLUE_TEXT));
    box(this->game_window, 0, 0);
    wattroff(this->game_window, COLOR_PAIR(BLUE_TEXT));

    // Rendering the apple
    wattron(this->game_window, COLOR_PAIR(RED_TEXT) | A_BOLD);
    Snake::Coordinates apple_position = this->game->get_apple_position();
    mvwaddch(this->game_window, apple_position.y, apple_position.x, 'o');
    wattroff(this->game_window, COLOR_PAIR(RED_TEXT) | A_BOLD);

    // Rendering the snake
    wattron(this->game_window, COLOR_PAIR(GREEN_TEXT));
    Snake::Coordinates snake_head = this->game->get_snake_body()->get_head()->position;
    mvwaddch(this->game_window, snake_head.y, snake_head.x,
             '@'); // @ head (ACS characters display incorrectly)

    for (uint16_t i = 1; i < this->game->get_snake_body()->size(); ++i) {
        Snake::Coordinates coord = this->game->get_snake_body()->get_element_at(i)->position;
        mvwaddch(this->game_window, coord.y, coord.x, '#'); // # body
    }
    wattroff(this->game_window, COLOR_PAIR(GREEN_TEXT));
    wrefresh(this->game_window);
    refresh();
}

void GameUI::wait_for_user_win_screen() {
    wattron(window, A_BOLD | COLOR_PAIR(YELLOW_TEXT));
    mvwprintw(window, 0, 2, "Score: %5u", this->game->get_score());
    wattroff(window, COLOR_PAIR(YELLOW_TEXT));

    wattron(window, COLOR_PAIR(GREEN_TEXT));
    const char game_won_text[] = "GAME WON!!";
    mvwprintw(window, 0, getmaxx(window) / 2 - strlen(game_won_text), game_won_text);
    wattroff(window, A_BOLD | COLOR_PAIR(GREEN_TEXT));
}
} // namespace Graphics

#endif