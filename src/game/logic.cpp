#ifndef LOGIC_CPP
#define LOGIC_CPP

#include "logic.hpp"
#include <stdexcept>

namespace Snake {
bool coordinates_are_equal(Coordinates a, Coordinates b) {
    return a.x == b.x && a.y == b.y;
}


Game::Game(uint16_t table_height, uint16_t table_width) {
    this->result = GAME_UNFINISHED;

    this->game_table.height = table_height;
    this->game_table.width = table_width;

    this->current_direction = DIRECTION_UP;

    this->snake_head_position.x = table_width / 2;
    this->snake_head_position.y = table_height / 2;

    this->new_apple_position();
}

void Game::new_apple_position() {
}

GameResult Game::update_game(Direction player_input) {
    if(coordinates_are_equal(this->snake_head_position, this->apple_position)) {

        // TODO: the snake should grow

    }

    // if the desired direction is valid and is different from the current one
    if (player_input != this->current_direction && player_input != ~this->current_direction) {

        // TODO
        switch (player_input) {
        case DIRECTION_UP: {
            break;
        }
        case DIRECTION_DOWN: {
            break;
        }
        case DIRECTION_LEFT: {
            break;
        }
        case DIRECTION_RIGHT: {
            break;
        }
        default:
            throw std::invalid_argument("player_input should assume only values defined by the 'Direction' enum");
            break;
        }
    }

    

    return GAME_UNFINISHED;
}
} // namespace Snake

#endif