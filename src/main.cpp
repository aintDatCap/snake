#include "game/game_manager.hpp"
#include "graphics/graphics.hpp"
#include <algorithm>

List<Snake::LevelInfo> default_levels() {
    List<Snake::LevelInfo> levels;

    Snake::LevelInfo level_info{0};

    levels.add_element(level_info);
    levels.add_element(level_info);
    levels.add_element(level_info);
    levels.add_element(level_info);
    levels.add_element(level_info);
    levels.add_element(level_info);
    levels.add_element(level_info);
    levels.add_element(level_info);
    levels.add_element(level_info);
    levels.add_element(level_info);

    return levels;
}

int main(int, char **) {
    Graphics::start_ncurses();
    List<Snake::LevelInfo> levels = default_levels();

    uint16_t window_width, window_height;
    getmaxyx(stdscr, window_height, window_width);

    window_width = std::max<uint16_t>(window_width, 20);
    window_height = std::max<uint16_t>(window_height, 10);

    Snake::SnakeGameManager game_manager(window_width, window_height, levels);

    Graphics::stop_ncurses();
}
