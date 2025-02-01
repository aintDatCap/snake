#include "game/game_manager.hpp"
#include "game/level_list.hpp"
#include "game/logic.hpp"
#include "graphics/graphics.hpp"
#include <algorithm>
#include <cstdint>

Snake::LevelList *default_levels() {
    Snake::LevelList* levels = new Snake::LevelList;

    uint32_t id = 0;
    for (uint16_t i = 0; i < 8; i++) {
        levels->add_element(Snake::LevelInfo(0, i, Snake::DIFFICULTY_EASY, id++));
        levels->add_element(Snake::LevelInfo(0, i, Snake::DIFFICULTY_NORMAL, id++));
        levels->add_element(Snake::LevelInfo(0, i, Snake::DIFFICULTY_HARD, id++));
    }

    return levels;
}

int main(int, char **) {
    Graphics::start_ncurses();

    Snake::LevelList* level_list = Snake::LevelList::from_file("levels.bin");

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
