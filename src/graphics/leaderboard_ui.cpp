#ifndef LEADERBOARD_UI_CPP
#define LEADERBOARD_UI_CPP

#include "graphics/leaderboard_ui.hpp"
#include "game/logic.hpp"
#include "graphics/graphics.hpp"
#include <algorithm>
#include <cstdint>
#include <ncurses.h>

namespace Graphics {
LeaderboardUI::LeaderboardUI(uint16_t width, uint16_t height, Snake::LevelList *level_list) {
    erase();
    this->width = width;
    this->height = height;

    this->level_list = level_list;

    const uint16_t labels_size = 0;
    const uint16_t entries_size = level_list->get_element_count() * height / 6 + 3 * height/5 + height/2;
    this->window = newpad(entries_size + labels_size, width);

    box(this->window, 0, 0);
    scrollok(this->window, true);
    prefresh(this->window, 0, 0, 0, 0, height - 1, width - 1);
    render_leaderboard();
}

void LeaderboardUI::render_leaderboard() {

    size_t level_count = level_list->get_element_count();

    this->level_entries = new WINDOW *[level_count];

    uint32_t current_y = 0;
    // Difficulty easy
    this->diff_easy_label = new_bordered_subpad(this->window, this->height / 5, this->width, current_y, 0);
    put_centered_colored_text(diff_easy_label, "DIFFICULTY EASY", COLOR_GREEN);

    current_y += this->height / 5;

    uint32_t subpad_index = 0;
    for (uint32_t i = 0; i < level_count; ++i) {
        Snake::LevelInfo info = level_list->get_element_at(i)->info;
        if (info.difficulty == Snake::DIFFICULTY_EASY) {
            this->level_entries[subpad_index] =
                new_bordered_subpad(this->window, this->height / 6, width / 2, current_y, width / 4);

            char text[40];
            snprintf(text, sizeof(text), "Level: %d\tHigh score: %d", info.id, info.high_score);
            put_centered_text(this->level_entries[subpad_index], text);

            current_y += this->height / 6;
            subpad_index++;
        }
    }

    // Difficulty normal
    this->diff_normal_label = new_bordered_subpad(this->window, this->height / 5, this->width, current_y, 0);
    put_centered_colored_text(diff_normal_label, "DIFFICULTY NORMAL", COLOR_BLUE);

    current_y += this->height / 5;

    for (uint32_t i = 0; i < level_count; ++i) {
        Snake::LevelInfo info = level_list->get_element_at(i)->info;
        if (info.difficulty == Snake::DIFFICULTY_NORMAL) {
            this->level_entries[subpad_index] =
                new_bordered_subpad(this->window, this->height / 6, width / 2, current_y, width / 4);

            char text[40];
            snprintf(text, sizeof(text), "Level: %d\tHigh score: %d", info.id, info.high_score);
            put_centered_text(this->level_entries[subpad_index], text);

            current_y += this->height / 5;
            subpad_index++;
        }
    }

    // Difficulty hard
    this->diff_hard_label = new_bordered_subpad(this->window, this->height / 5, this->width, current_y, 0);
    put_centered_colored_text(diff_hard_label, "DIFFICULTY HARD", COLOR_RED);

    current_y += this->height / 5;

    for (uint32_t i = 0; i < level_count; ++i) {
        Snake::LevelInfo info = level_list->get_element_at(i)->info;
        if (info.difficulty == Snake::DIFFICULTY_HARD) {
            this->level_entries[subpad_index] =
                new_bordered_subpad(this->window, this->height / 6, width / 2, current_y, width / 4);

            char text[40];
            snprintf(text, sizeof(text), "Level: %d\tHigh score: %d", info.id, info.high_score);
            put_centered_text(this->level_entries[subpad_index], text);

            current_y += this->height / 5;
            subpad_index++;
        }
    }

    prefresh(this->window, 0, 0, 0, 0, height - 1, width - 1);
}

void LeaderboardUI::wait_for_user_input() {
    keypad(this->window, TRUE);

    uint32_t current_line = 0;

    while (true) {
        int c = wgetch(this->window);
        if (c == KEY_MOUSE) {
            MEVENT mouse_event;
            if (getmouse(&mouse_event) == OK) {
                // left button clicked
                if (mouse_event.bstate & BUTTON4_PRESSED) {

                    current_line -= std::min<uint32_t>(2, current_line);
                    prefresh(this->window, current_line, 0, 0, 0, height - 1, width - 1);

                } else if (mouse_event.bstate & BUTTON5_PRESSED) {
                    // avoid thinking too hard on why it works
                    current_line += std::min<uint32_t>(2, (uint32_t)getmaxy(this->window) - (current_line + height));
                    prefresh(this->window, current_line, 0, 0, 0, height - 1, width - 1);
                }
            }
        } else if(c=='q') {
            return;
        }
    }
}

LeaderboardUI::~LeaderboardUI() {
    delwin(this->diff_easy_label);
    delwin(this->diff_normal_label);
    delwin(this->diff_hard_label);

    for (uint16_t i = 0; i < level_list->get_element_count(); i++) {
        delwin(this->level_entries[i]);
    }
    delwin(this->window);
}
} // namespace Graphics

#endif