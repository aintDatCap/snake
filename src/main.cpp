#include "game/game_manager.hpp"
#include "game/level_list.hpp"
#include "game/logic.hpp"
#include "graphics/graphics.hpp"
#include <algorithm>
#include <cstdint>

Snake::LevelList *default_levels() {
    Snake::LevelList* levels = new Snake::LevelList;

    for (uint16_t i = 1; i <=8; i++) {
        levels->add_element(Snake::LevelInfo(0, i, Snake::DIFFICULTY_EASY));
        levels->add_element(Snake::LevelInfo(0, i, Snake::DIFFICULTY_NORMAL));
        levels->add_element(Snake::LevelInfo(0, i, Snake::DIFFICULTY_HARD));
    }

    return levels;
}

int main(int, char **) {
    Graphics::start_ncurses();

    Snake::LevelList* level_list = Snake::LevelList::from_file(LEVELS_FILE_NAME);

    if(!level_list) {
        level_list = default_levels();
    }

    uint16_t window_width, window_height;
    getmaxyx(stdscr, window_height, window_width);

    window_width = std::max<uint16_t>(window_width, 20);
    window_height = std::max<uint16_t>(window_height, 10);

    Snake::SnakeGameManager game_manager(window_width, window_height, level_list);

    Graphics::stop_ncurses();
}
