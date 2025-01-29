#ifndef GRAPHICS_CPP
#define GRAPHICS_CPP

#include "graphics/graphics.hpp"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <ncurses.h>
#include <string.h>

namespace Graphics {
void put_centered_text(WINDOW *win, const char *text) {
    mvwprintw(win, getmaxy(win) / 2, (getmaxx(win) - strlen(text)) / 2, "%s", text);
}

void put_centered_colored_text(WINDOW *win, const char *text, int color_id) {
    wattron(win, COLOR_PAIR(color_id));
    put_centered_text(win, text);
    wattroff(win, COLOR_PAIR(color_id));
}

bool is_inside_window(WINDOW *win, int x, int y) {
    return (getbegx(win) <= x && (getbegx(win) + getmaxx(win)) >= x) &&
           (getbegy(win) <= y && (getbegy(win) + getmaxy(win)) >= y);
}

bool is_inside_subpad(WINDOW *subpad, int x, int y, int current_parent_line) {
    return (getparx(subpad) <= x && (getparx(subpad) + getmaxx(subpad)) >= x) &&
           ((getpary(subpad) - current_parent_line) <= y &&
            (getpary(subpad) + getmaxy(subpad) - current_parent_line) >= y);
}

WINDOW *new_bordered_window(uint16_t height, uint16_t width, uint16_t y, uint16_t x) {
    WINDOW *window = newwin(height, width, y, x);
    refresh();
    box(window, 0, 0);
    wrefresh(window);
    return window;
}

void start_ncurses() {
    initscr();
    start_color();

    init_pair(Graphics::RED_TEXT, COLOR_RED, COLOR_BLACK);
    init_pair(Graphics::GREEN_TEXT, COLOR_GREEN, COLOR_BLACK);
    init_pair(Graphics::BLUE_TEXT, COLOR_BLUE, COLOR_BLACK);

    mousemask(ALL_MOUSE_EVENTS, NULL); // for mouse events...

    noecho(); // No keys on the screen
    curs_set(0);
}

void stop_ncurses() {
    erase();
    endwin();
}

} // namespace Graphics

#endif