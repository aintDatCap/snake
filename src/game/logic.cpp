#ifndef LOGIC_CPP
#define LOGIC_CPP

#include "logic.hpp"
#include <cstdint>
#include <stdexcept>

#define SNAKE_BODY_SIZE 4

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

    for (uint16_t i = this->snake_head_position.y + 1; i < this->snake_head_position.y + SNAKE_BODY_SIZE; i++) {
        Coordinates coords;
        coords.x = this->snake_head_position.x;
        coords.y = i;
        this->snake_body.append(coords);
    }
}

void Game::new_apple_position() {
    // TODO
    this->apple_position.x = 3;
    this->apple_position.y = 3;
}

GameResult Game::update_game(Direction player_input) {
    if (coordinates_are_equal(this->snake_head_position, this->apple_position)) {
        // TODO: update player score and create a new apple
        this->new_apple_position();
    }

    // if the desired direction is valid and is different from the current one
    // we should change the current direction based on player input
    if (player_input != this->current_direction && player_input != ~this->current_direction) {
        // set this->current_direction
    }

    switch (player_input) {
    case DIRECTION_UP: {
        // remove 1 to y
        break;
    }
    case DIRECTION_DOWN: {
        // add 1 to y
        break;
    }
    case DIRECTION_LEFT: {
        // remove 1 to x
        break;
    }
    case DIRECTION_RIGHT: {
        // add 1 to x
        break;
    }
    default: {
        throw std::invalid_argument("player_input should assume only values defined by the 'Direction' enum");
        break;
    }
    }

    return GAME_UNFINISHED;
}
} // namespace Snake

#endif