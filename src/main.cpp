#include "game/game_manager.hpp"
#include "game/graphics.hpp"
#include "game/logic.hpp"

int main(int, char **) {
    start_ncurses();
    List<Snake::LevelInfo> levels;
    Snake::SnakeGameManager game_manager(100, 30, levels);
    
    stop_ncurses();
}
