#ifndef LOGIC_CPP
#define LOGIC_CPP

#include "logic.hpp"
#include <cstdlib>

namespace Snake {
bool coordinates_are_equal(Coordinates a, Coordinates b) {
    return a.x == b.x && a.y == b.y;
}

GameTable get_playable_dimensions(GameDifficulty difficulty) {
    switch (difficulty) {
        case DIFFICULTY_EASY:
            return {30, 80};
        case DIFFICULTY_NORMAL:
            return {25, 70};
        case DIFFICULTY_HARD:
            return {20, 60};
        default:
            return {25, 70};
    }
}

} // namespace Snake

#endif