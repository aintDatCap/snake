#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <cstdint>
#include <ctime>

#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

namespace Graphics {

typedef enum {
    RED_TEXT = 1,
    GREEN_TEXT = 2,
    BLUE_TEXT = 3,
    YELLOW_TEXT = 4
} UIColors;

/**
 * Puts centered text inside of a window
 */
void put_centered_text(WINDOW *win, const char *text);

/**
 * Puts centered text inside of a window
 * that has a certain color
 */
void put_centered_colored_text(WINDOW *win, const char *text, int color_id);

/**
 * Returns true if the given coordinates are inside of the given window
 */
bool is_inside_window(WINDOW *win, int x, int y);

/**
 * Returns true if the given coordinates are inside of the given subpad
 * requires the parent's current line
 */
bool is_inside_subpad(WINDOW *subpad, int x, int y, int current_parent_line);

void start_ncurses();
void stop_ncurses();

WINDOW *new_bordered_window(uint16_t height, uint16_t width, uint16_t y, uint16_t x);
WINDOW *new_bordered_subpad(WINDOW* parent, uint16_t height, uint16_t width, uint16_t y, uint16_t x);

} // namespace Graphics

#endif