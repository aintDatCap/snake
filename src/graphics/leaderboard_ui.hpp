#ifndef LEADERBOARD_UI_HPP
#define LEADERBOARD_UI_HPP

#include "game/level_list.hpp"
#include <cstdint>
#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

namespace Graphics {
class LeaderboardUI {
  private:
    uint32_t width;
    uint32_t height;
    WINDOW *window;
    WINDOW *leaderboard_win;
    WINDOW *diff_easy_label;
    WINDOW *diff_normal_label;
    WINDOW *diff_hard_label;
    WINDOW **level_entries;
    Snake::LevelList *level_list;

    void render_leaderboard();
    
  public:
    LeaderboardUI(uint16_t width, uint16_t height, Snake::LevelList *level_list);
    ~LeaderboardUI();

    void wait_for_user_input();
};
} // namespace Graphics

#endif