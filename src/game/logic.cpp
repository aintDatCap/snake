#ifndef LOGIC_CPP
#define LOGIC_CPP

#include "logic.hpp"
#include "utils/queue.hpp"
#include <cstdint>
#include <cstdlib>
#include <stdexcept>

#define SNAKE_BODY_SIZE 4

namespace Snake {
bool coordinates_are_equal(Coordinates a, Coordinates b) {
    return a.x == b.x && a.y == b.y;
}

Game::Game(uint16_t table_height, uint16_t table_width, GameDifficulty game_difficulty) {
    this->game_difficulty = game_difficulty;
    this->result = GAME_UNFINISHED;

    this->game_table.height = table_height;
    this->game_table.width = table_width;

    this->current_direction = DIRECTION_UP;

    this->snake_head_position.x = table_width / 2;
    this->snake_head_position.y = table_height / 2;

    this->snake_body = new Queue<Coordinates>();
    for (uint16_t i = this->snake_head_position.y + 1; i < this->snake_head_position.y + SNAKE_BODY_SIZE; i++) {
        Coordinates coords;
        coords.x = this->snake_head_position.x;
        coords.y = i;
        this->snake_body->enqueue(coords);
    }

    this->new_apple_position();
}

void Game::new_apple_position() {
    bool valid_position = false;
    while (!valid_position) {
        // Generate random coordinates within the bounds of the playable area
        this->apple_position.x = rand() % this->game_table.width;
        this->apple_position.y = rand() % this->game_table.height;

        // Check if the apple position is valid, i.e. the apple
        // does not overlap with the snake's body

        valid_position = true;
        for (uint16_t i = 0; i < this->snake_body->size(); ++i) {
            auto body_part_element = this->snake_body->get_element_at(i);
            if (body_part_element && coordinates_are_equal(this->apple_position, body_part_element->value)) {
                valid_position = false;
                break;
            }
        }
    }
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