#include "game/graphics.hpp"
#include "game/logic.hpp"

int main(int, char **) {
    start_ncurses();

    Snake::Game *game = new Snake::Game(25, 100);
    Snake::GameUI ui(game);

    ui.update_game_window();


    getch();
    
    stop_ncurses();
}
