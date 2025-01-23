#include "game/game_manager.hpp"
#include "game/graphics.hpp"

#define max(a, b) a > b ? a : b

List<Snake::LevelInfo> default_levels() {
    List<Snake::LevelInfo> levels;

Snake::LevelInfo level_info{0};

    levels.add_element(
        level_info
    );
        levels.add_element(
        level_info
    );
        levels.add_element(
        level_info
    );
        levels.add_element(
        level_info
    );
        levels.add_element(
        level_info
    );
        levels.add_element(
        level_info
    );

    return levels;
}

int main(int, char **) {
    start_ncurses();
    List<Snake::LevelInfo> levels;

    uint16_t window_width, window_height;
    getmaxyx(stdscr, window_height, window_width);

    window_width = max(window_width, 20);
    window_height = max(window_height, 10);

    Snake::SnakeGameManager game_manager(window_width, window_height, levels);

    stop_ncurses();
}
