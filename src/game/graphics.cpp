#ifndef GRAPHICS_CPP
#define GRAPHICS_CPP

#include "graphics.hpp"
#include "ncurses/ncurses.h"

namespace Snake {
    UI::UI(Game *game) {
        this->game = game;
        // height, width, posy, posx
        this->window = newwin(300, 700, 0, 0);

    }

    UI::~UI(){
        delwin(this->window);
    }
}

#endif