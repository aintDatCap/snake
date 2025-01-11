#include "game/game_manager.hpp"
#include "game/graphics.hpp"

int main(int, char **) {
    start_ncurses();
    List<Snake::LevelInfo> levels;

    uint16_t window_width, window_height;
    getmaxyx(stdscr, window_height, window_width);

    Snake::SnakeGameManager game_manager(window_width, window_height, levels);
    
    stop_ncurses();
}
