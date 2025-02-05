#ifndef LEADERBOARD_UI_CPP
#define LEADERBOARD_UI_CPP

#include "graphics/leaderboard_ui.hpp"
#include "game/logic.hpp"
#include "graphics/graphics.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <ncurses.h>

namespace Graphics {
LeaderboardUI::LeaderboardUI(uint16_t width, uint16_t height, Snake::LevelList *level_list) {
    erase();
    this->width = width;
    this->height = height;

    this->level_list = level_list;

    this->window = newwin(height, width, 0, 0);
    refresh();
    const char text[] = "Press Q to quit";
    mvwprintw(this->window, height * (0.95), (getmaxx(this->window) - strlen(text)) / 2, text);

    wrefresh(this->window);

    const uint16_t labels_size = this->height / 5 * 4;
    const uint16_t entries_size = (level_list->get_element_count() + 1) * height / 6;
    this->leaderboard_win = newpad(entries_size + labels_size, width - 2);

    box(leaderboard_win, 0, 0);
    scrollok(this->leaderboard_win, true);
    prefresh(this->leaderboard_win, 0, 0, 1, 1, height * (0.9), width - 2);
    render_leaderboard();
}

void LeaderboardUI::render_leaderboard() {

    size_t level_count = level_list->get_element_count();

    this->level_entries = new WINDOW *[level_count];

    uint32_t current_y = 0;
    // Difficulty easy
    this->diff_easy_label = new_colored_bordered_subpad(this->leaderboard_win, this->height / 5, this->width - 2,
                                                        current_y, 0, COLOR_GREEN);
    put_centered_colored_text(diff_easy_label, "DIFFICULTY EASY", COLOR_GREEN);

    current_y += this->height / 5;

    uint32_t subpad_index = 0;
    for (uint32_t i = 0; i < level_count; ++i) {
        Snake::LevelInfo info = level_list->get_element_at(i)->info;
        if (info.difficulty == Snake::DIFFICULTY_EASY) {
            this->level_entries[subpad_index] =
                new_bordered_subpad(this->leaderboard_win, this->height / 6, width / 2, current_y, width / 4);

            char text[40];
            snprintf(text, sizeof(text), "Level: %d\tHigh score: %d", info.id, info.high_score);
            put_centered_text(this->level_entries[subpad_index], text);

            current_y += this->height / 6;
            subpad_index++;
        }
    }

    // Difficulty normal
    this->diff_normal_label = new_colored_bordered_subpad(this->leaderboard_win, this->height / 5, this->width - 2,
                                                          current_y, 0, COLOR_GREEN);
    put_centered_colored_text(diff_normal_label, "DIFFICULTY NORMAL", COLOR_BLUE);

    current_y += this->height / 5;

    for (uint32_t i = 0; i < level_count; ++i) {
        Snake::LevelInfo info = level_list->get_element_at(i)->info;
        if (info.difficulty == Snake::DIFFICULTY_NORMAL) {
            this->level_entries[subpad_index] =
                new_bordered_subpad(this->leaderboard_win, this->height / 6, width / 2, current_y, width / 4);

            char text[40];
            snprintf(text, sizeof(text), "Level: %d\tHigh score: %d", info.id, info.high_score);
            put_centered_text(this->level_entries[subpad_index], text);

            current_y += this->height / 5;
            subpad_index++;
        }
    }

    // Difficulty hard
    this->diff_hard_label =
        new_colored_bordered_subpad(this->leaderboard_win, this->height / 5, this->width - 2, current_y, 0, COLOR_RED);
    put_centered_colored_text(diff_hard_label, "DIFFICULTY HARD", COLOR_RED);

    current_y += this->height / 5;

    for (uint32_t i = 0; i < level_count; ++i) {
        Snake::LevelInfo info = level_list->get_element_at(i)->info;
        if (info.difficulty == Snake::DIFFICULTY_HARD) {
            this->level_entries[subpad_index] =
                new_bordered_subpad(this->leaderboard_win, this->height / 6, width / 2, current_y, width / 4);

            char text[40];
            snprintf(text, sizeof(text), "Level: %d\tHigh score: %d", info.id, info.high_score);
            put_centered_text(this->level_entries[subpad_index], text);

            current_y += this->height / 5;
            subpad_index++;
        }
    }

    prefresh(this->leaderboard_win, 0, 0, 1, 1, height * (0.9), width - 2);
}

void LeaderboardUI::wait_for_user_input() {
    keypad(this->leaderboard_win, TRUE);

    uint32_t current_line = 0;

    while (true) {
        int c = wgetch(this->leaderboard_win);
        if (c == KEY_MOUSE) {
            MEVENT mouse_event;
            if (getmouse(&mouse_event) == OK) {
                // left button clicked
                if (mouse_event.bstate & BUTTON4_PRESSED) {
                    current_line -= std::min<uint32_t>(2, current_line);

                    prefresh(this->leaderboard_win, current_line, 0, 1, 1, height * (0.9), width - 2);
                } else if (mouse_event.bstate & BUTTON5_PRESSED) {
                    // avoid thinking too hard on why it works
                    current_line += std::min<uint32_t>(2, (uint32_t)getmaxy(this->leaderboard_win) -
                                                              (current_line + height * (0.9) - 1));

                    prefresh(this->leaderboard_win, current_line, 0, 1, 1, height * (0.9), width - 2);
                }
            }
        } else if (c == 'q') {
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
    delwin(this->leaderboard_win);
}
} // namespace Graphics

#endif