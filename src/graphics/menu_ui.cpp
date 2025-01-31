#ifndef MENU_UI_CPP
#define MENU_UI_CPP

#include "graphics/menu_ui.hpp"
#include "graphics/graphics.hpp"

namespace Graphics {

MenuUI::MenuUI(uint16_t width, uint16_t height) {
    this->player_selection.game_difficulty = Snake::DIFFICULTY_NORMAL;

    this->window = new_bordered_window(height, width, (LINES - height) / 2, (COLS - width) / 2);

    // Button dimensions and spacing
    const int button_height = height / 6;
    const int button_width = width / 3;
    const int vertical_spacing = height / 6; 

    // Play button 
    this->play_game_button =
        new_bordered_window(button_height, button_width, height / 2 - 2 * vertical_spacing, (width - button_width) / 2);
    wattron(play_game_button, A_BOLD);
    put_centered_colored_text(play_game_button, "Play", GREEN_TEXT);
    wattroff(play_game_button, A_BOLD);
    wrefresh(play_game_button);

    // Difficulty button 
    this->difficulty_button = new_bordered_window(button_height, button_width, height / 2 - vertical_spacing, (width - button_width) / 2);
    render_difficulty_button();

    // Leaderboard button
    this->leaderboard_button =
        new_bordered_window(button_height, button_width, height / 2, (width - button_width) / 2);
    wattron(leaderboard_button, A_BOLD);
    put_centered_colored_text(leaderboard_button, "Leaderboard", YELLOW_TEXT);
    wattroff(leaderboard_button, A_BOLD);
    wrefresh(leaderboard_button);

    // Exit button (position adjusted downward)
    this->exit_button =
        new_bordered_window(button_height, button_width, height / 2 + vertical_spacing, (width - button_width) / 2);
    wattron(exit_button, A_BOLD);
    put_centered_colored_text(exit_button, "Exit", RED_TEXT);
    wattroff(exit_button, A_BOLD);
    wrefresh(exit_button);

    refresh();
}

void MenuUI::render_difficulty_button() {
    const char *difficulty_text = "";
    UIColors color = BLUE_TEXT;

    switch (this->player_selection.game_difficulty) {
        case Snake::DIFFICULTY_EASY:
            difficulty_text = "Easy";
            color = GREEN_TEXT;
            break;
        case Snake::DIFFICULTY_NORMAL:
            difficulty_text = "Normal";
            color = BLUE_TEXT;
            break;
        case Snake::DIFFICULTY_HARD:
            difficulty_text = "Hard";
            color = RED_TEXT;
            break;
    }

    werase(difficulty_button);
    box(difficulty_button, 0, 0);

    wattron(difficulty_button, COLOR_PAIR(color) | A_BOLD);
    put_centered_text(difficulty_button, difficulty_text);
    wattroff(difficulty_button, COLOR_PAIR(color) | A_BOLD);

    wrefresh(difficulty_button);
}

MenuUI::~MenuUI() {
    delwin(this->window);
    delwin(this->play_game_button);
    delwin(this->difficulty_button);
    delwin(this->leaderboard_button);
    delwin(this->exit_button);
    refresh();
}

MenuUIAction MenuUI::wait_for_user_input() {
    keypad(this->window, TRUE);

    while (true) {
        int c = wgetch(this->window);
        if (c == KEY_MOUSE) {
            MEVENT mouse_event;
            if (getmouse(&mouse_event) == OK) {
                // left button clicked
                if (mouse_event.bstate & BUTTON1_CLICKED || mouse_event.bstate & BUTTON1_PRESSED) {

                    if (is_inside_window(play_game_button, mouse_event.x, mouse_event.y)) {
                        // Instead of directly starting the game, go to the level selector
                        player_selection.action = MENU_SELECT_LEVEL;
                        return player_selection;
                    } else if (is_inside_window(difficulty_button, mouse_event.x, mouse_event.y)) {

                        switch (player_selection.game_difficulty) {
                            case Snake::DIFFICULTY_EASY:
                                player_selection.game_difficulty = Snake::DIFFICULTY_NORMAL;
                                break;
                            case Snake::DIFFICULTY_NORMAL:
                                player_selection.game_difficulty = Snake::DIFFICULTY_HARD;
                                break;
                            case Snake::DIFFICULTY_HARD:
                                player_selection.game_difficulty = Snake::DIFFICULTY_EASY;
                                break;
                        }
                        render_difficulty_button();
                    } else if (is_inside_window(exit_button, mouse_event.x, mouse_event.y)) {
                        player_selection.action = MENU_EXIT_PROGRAM;
                        return player_selection;
                    } else if (is_inside_window(leaderboard_button, mouse_event.x, mouse_event.y)) {
                        player_selection.action = MENU_LEADERBOARD;
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
} // namespace Graphics

#endif