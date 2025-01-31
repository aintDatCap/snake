#ifndef PAUSE_UI_CPP
#define PAUSE_UI_CPP

#include "graphics/pause_ui.hpp"
#include "graphics/graphics.hpp"

namespace Graphics {

PauseUI::PauseUI(uint16_t width, uint16_t height) {
    this->window = new_bordered_window(height, width,
                                       (LINES - height) / 2, // Center vertically
                                       (COLS - width) / 2    // Center horizontally
    );

    // Calculate button positions
    const int button_height = height / 6;
    const int button_width = width / 3;
    const int vertical_spacing = height / 5;

    // Resume button
    this->resume_button =
        new_bordered_window(button_height, button_width, height / 2 - vertical_spacing, (width - button_width) / 2);
    wattron(resume_button, A_BOLD);
    put_centered_colored_text(resume_button, "Resume", GREEN_TEXT);
    wattroff(resume_button, A_BOLD);
    wrefresh(resume_button);

    // Level Selector button
    // this->level_selector_button =
    //     new_bordered_window(button_height, button_width, height / 2, (width - button_width) / 2);
    // wattron(level_selector_button, A_BOLD);
    // put_centered_colored_text(level_selector_button, "Level Selector", BLUE_TEXT);
    // wattroff(level_selector_button, A_BOLD);
    // wrefresh(level_selector_button);

    // Exit button
    this->exit_button = new_bordered_window(button_height, button_width, height / 2, (width - button_width) / 2);
    wattron(exit_button, A_BOLD);
    put_centered_colored_text(exit_button, "Exit", RED_TEXT);
    wattroff(exit_button, A_BOLD);
    wrefresh(exit_button);

    refresh();
}

PauseUI::~PauseUI() {
    delwin(this->resume_button);
    // delwin(this->level_selector_button);
    delwin(this->exit_button);
    delwin(this->window);
    refresh();
}

PauseUIAction PauseUI::wait_for_user_input() {
    keypad(this->window, TRUE);

    while (true) {
        int c = wgetch(this->window);
        if (c == KEY_MOUSE) {
            MEVENT mouse_event;
            if (getmouse(&mouse_event) == OK) {
                if (mouse_event.bstate & (BUTTON1_CLICKED | BUTTON1_PRESSED)) {
                    if (is_inside_window(resume_button, mouse_event.x, mouse_event.y)) {
                        player_selection.action = PAUSE_RESUME;
                        return player_selection;
                    } else if (is_inside_window(exit_button, mouse_event.x, mouse_event.y)) {
                        player_selection.action = PAUSE_EXIT_PROGRAM;
                        return player_selection;
                    }
                    //  else if (is_inside_window(level_selector_button, mouse_event.x, mouse_event.y)) {
                    //     player_selection.action = PAUSE_SELECT_LEVEL;
                    //     return player_selection;
                    //  }
                }
            }
        } else if (c == KEY_EXIT) {
            player_selection.action = PAUSE_EXIT_PROGRAM;
            return player_selection;
        }
    }
}
} // namespace Graphics

#endif