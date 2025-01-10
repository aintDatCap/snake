#include "game/graphics.hpp"
#include "game/logic.hpp"
#include "ncurses/ncurses.h"

int main(int, char **) {


    Snake::Game *game = new Snake::Game(25, 100);
    Snake::UI ui(game);

    ui.update_game_window();


    getch();
    endwin();
}
