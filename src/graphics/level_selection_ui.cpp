#ifndef LEVEL_SELECTION_UI_CPP
#define LEVEL_SELECTION_UI_CPP

#include "graphics/level_selection_ui.hpp"
#include "game/level_list.hpp"
#include "graphics/graphics.hpp"
#include <algorithm>
#include <cstring>
#include <ncurses.h>

namespace Graphics {

// -LevelSelctorUI definitions
LevelSelectionUI::LevelSelectionUI(uint16_t width, uint16_t height, Snake::LevelList *levels, Snake::GameDifficulty selected_difficulty) {
    erase();
    this->width = width;
    this->height = height;

    this->levels = levels;
    this->selected_difficulty = selected_difficulty;

    this->window = newwin(height, width, 0, 0);
    refresh();
    const char text[] = "Press Q to quit";
    mvwprintw(this->window, height * (0.95), (getmaxx(this->window) - strlen(text)) / 2, text);
    wrefresh(this->window);

    this->level_selection_win = newpad((levels->get_element_count(selected_difficulty) + 2) * height / 6, width - 2);

    box(this->level_selection_win, 0, 0);
    scrollok(this->level_selection_win, true);
    prefresh(this->level_selection_win, 0, 0, 0, 0, height*0.9, width - 2);

    render_level_buttons();
}

void LevelSelectionUI::render_level_buttons() {
    const size_t level_count = levels->get_element_count(selected_difficulty);

    this->level_buttons = new WINDOW *[level_count];

    for (uint32_t i = 0; i < level_count; ++i) {
        // Coordinates
        uint16_t x = (width - width / 3) / 2;
        uint16_t y = ((i + 1) * height) / 6;
        uint16_t btn_width = width / 3;
        uint16_t btn_height = height / 8;

        level_buttons[i] = subpad(this->level_selection_win, btn_height, btn_width, y, x);
        box(level_buttons[i], 0, 0);

        char level_text[10];
        snprintf(level_text, sizeof(level_text), "Level %d", i + 1);
        put_centered_text(level_buttons[i], level_text);
    }
    prefresh(this->level_selection_win, 0, 0, 0, 0, height*0.9, width - 2);
}

LevelSelection LevelSelectionUI::wait_for_level_input() {
    keypad(this->level_selection_win, TRUE);

    uint32_t current_line = 0;

    while (true) {
        int c = wgetch(this->level_selection_win);
        if (c == KEY_MOUSE) {
            MEVENT mouse_event;
            if (getmouse(&mouse_event) == OK) {
                // left button clicked
                if (mouse_event.bstate & BUTTON1_CLICKED || mouse_event.bstate & BUTTON1_PRESSED) {
                    for (uint32_t i = 0; i < levels->get_element_count(selected_difficulty); ++i) {
                        if (is_inside_subpad(level_buttons[i], mouse_event.x, mouse_event.y, (int32_t)current_line)) {
                            this->level_selection.action = LEVEL_SELECT_PLAY;
                            this->level_selection.level = i + 1;
                            return this->level_selection; // Save the value of the selected level (1,2, etc.)
                        }
                    }
                } else if (mouse_event.bstate & BUTTON4_PRESSED) {

                    current_line -= std::min<uint32_t>(2, current_line);
                    prefresh(this->level_selection_win, current_line, 0, 0, 0, height*0.9, width - 2);

                } else if (mouse_event.bstate & BUTTON5_PRESSED) {
                    // avoid thinking too hard on why it works
                    current_line += std::min<uint32_t>(2, (uint32_t)getmaxy(this->level_selection_win) - (current_line + height*0.9 - 1));
                    prefresh(this->level_selection_win, current_line, 0, 0, 0, height *0.9, width - 2);
                }
            }
        } else if(c == 'q') {
            this->level_selection.action = LEVEL_SELECT_EXIT;
            return this->level_selection;
        }
    }
}

// Destructor
LevelSelectionUI::~LevelSelectionUI() {
    for (uint32_t i = 0; i < levels->get_element_count(selected_difficulty); ++i) {
        delwin(this->level_buttons[i]);
    }
    delwin(this->level_selection_win);
    delete this->level_buttons;
    delwin(this->window);
    refresh();
}
} // namespace Graphics

#endif