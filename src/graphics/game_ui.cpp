#ifndef GAME_UI_CPP
#define GAME_UI_CPP

#include "graphics/game_ui.hpp"
#include "graphics/graphics.hpp"

namespace Graphics {
GameUI::GameUI(Snake::Game *game) {
    this->game = game;

    Snake::GameTable game_table = game->get_game_table();

    this->window = new_bordered_window(game_table.height, game_table.width, 0, 0);
}

GameUI::~GameUI() {
    delwin(this->window);
    refresh();
}

WINDOW *GameUI::getWindow() {
    return this->window;
}

void GameUI::update_game_window(int32_t remaining_time) {
    // Clear the window
    werase(this->window);
    // box(this->window, 0, 0);

    // Rendering the time and score
    wattron(window, A_BOLD);
    mvwprintw(window, 0, 2, "Score: %u", this->game->get_score());
    mvwprintw(window, 0, getmaxx(window) - 12, "Time: %i", remaining_time);
    wattroff(window, A_BOLD);

    // Get the playable area size based on difficulty
    Snake::GameTable playable_area = this->game->get_playable_area();

    // Calculate the position of the inner border
    int start_x = (getmaxx(window) - playable_area.width) / 2;
    int start_y = (getmaxy(window) - playable_area.height) / 2;

    // Draw the inner border
    wattron(window, COLOR_PAIR(BLUE_TEXT));
    for (int x = start_x; x < start_x + playable_area.width; x++) {
        mvwaddch(window, start_y, x, ACS_HLINE);
        mvwaddch(window, start_y + playable_area.height - 1, x, ACS_HLINE);
    }
    for (int y = start_y; y < start_y + playable_area.height; y++) {
        mvwaddch(window, y, start_x, ACS_VLINE);
        mvwaddch(window, y, start_x + playable_area.width - 1, ACS_VLINE);
    }
    mvwaddch(window, start_y, start_x, ACS_ULCORNER);
    mvwaddch(window, start_y, start_x + playable_area.width - 1, ACS_URCORNER);
    mvwaddch(window, start_y + playable_area.height - 1, start_x, ACS_LLCORNER);
    mvwaddch(window, start_y + playable_area.height - 1, start_x + playable_area.width - 1, ACS_LRCORNER);
    wattroff(window, COLOR_PAIR(BLUE_TEXT));

    // Rendering the apple
    wattron(window, COLOR_PAIR(RED_TEXT) | A_BOLD);
    Snake::Coordinates apple_position = this->game->get_apple_position();
    mvwaddch(window, start_y + apple_position.y, start_x + apple_position.x, 'o');
    wattroff(window, COLOR_PAIR(RED_TEXT) | A_BOLD);

    // Rendering the snake
    wattron(this->window, COLOR_PAIR(GREEN_TEXT));
    Snake::Coordinates snake_head = this->game->get_snake_head_position();
    mvwaddch(this->window, start_y + snake_head.y, start_x + snake_head.x,
             '@'); // @ head (ACS characters display incorrectly)

    for (uint16_t i = 0; i < this->game->get_snake_body()->size(); ++i) {
        Snake::Coordinates coord = this->game->get_snake_body()->get_element_at(i)->value;
        mvwaddch(this->window, start_y + coord.y, start_x + coord.x, '#'); // # body
    }
    wattroff(this->window, COLOR_PAIR(GREEN_TEXT));
    wrefresh(this->window);
    refresh();
}
} // namespace Graphics

#endif