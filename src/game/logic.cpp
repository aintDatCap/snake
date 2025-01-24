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
    for (uint16_t i = this->snake_head_position.y + 1;
         i < this->snake_head_position.y + SNAKE_BODY_SIZE + game_difficulty; i++) {
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
        // Generate random coordinates within the bounds of the playable area,
        // but avoid the borders (first and last rows/columns)
        this->apple_position.x = rand() % (this->game_table.width - 2) + 1;  // Avoid left and right borders
        this->apple_position.y = rand() % (this->game_table.height - 2) + 1; // Avoid top and bottom borders

        // Check if the apple position is valid, i.e., the apple
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

uint32_t Game::calculate_points(uint16_t level, GameDifficulty difficulty) const {
    uint32_t base_points = 10; // Points awarded for eating an apple
    uint32_t difficulty_multiplier = 1;

    // Change multiplier based on difficulty
    switch (difficulty) {
    case DIFFICULTY_EASY:
        difficulty_multiplier = 1;
        break;
    case DIFFICULTY_NORMAL:
        difficulty_multiplier = 2;
        break;
    case DIFFICULTY_HARD:
        difficulty_multiplier = 3;
        break;
    default:
        throw std::invalid_argument("Invalid game difficulty");
    }

    return base_points * difficulty_multiplier * level;
}

void Game::set_speed(uint32_t level) {
    switch (this->game_difficulty) {
        // the game is made harder by making the snake move every
        // unit of time expressed in milliseconds
        // the lower the time intervals the harder the game
    case DIFFICULTY_EASY:           // tentative values for speed before playtesting
        speed = 300000 - (level * 10000); // Lower speed
        break;
    case DIFFICULTY_NORMAL:
        speed = 200000 - (level * 15000); // Moderate speed
        break;
    case DIFFICULTY_HARD:
        speed = 100000 - (level * 20000); // Faster speed
        break;
    default:
        speed = 125000; // Default speed
        break;
    }

    if (speed < 50000) {
        speed = 50000; // Cap the speed at a minimum interval (e.g., 50 ms)
    }
}

GameResult Game::update_game(Direction player_input) {
    if (this->result != GAME_UNFINISHED) {
        return this->result;
    }

    if (coordinates_are_equal(this->snake_head_position, this->apple_position)) {
        // TODO: update player score and create a new apple
        this->new_apple_position();
    }

    // if the desired direction is valid and is different from the current one
    // we should change the current direction based on player input
    if (player_input != DIRECTION_NONE && player_input != this->current_direction &&
        player_input != ~this->current_direction) {
        // the new direction is valid
        this->current_direction = player_input;
    }

    // move the tail
    snake_body->dequeue();
    snake_body->enqueue(snake_head_position);

    // move the head
    switch (this->current_direction) {
    case DIRECTION_UP: {
        snake_head_position.y--;
        
        if (snake_head_position.y == 0) {
            return GAME_LOST;
        }
        break;
    }
    case DIRECTION_DOWN: {
        // add 1 to y
        snake_head_position.y++;

        if (snake_head_position.y == game_table.height) { // -1 because of the border
            return GAME_LOST;
        }
        break;
    }
    case DIRECTION_LEFT: {
        // remove 1 to x
        snake_head_position.x--;
        if (snake_head_position.x == 0) {
            return GAME_LOST;
        }
        break;
    }
    case DIRECTION_RIGHT: {
        // add 1 to x
        snake_head_position.x++;

        if (snake_head_position.x == game_table.width) { 
            return GAME_LOST;
        }
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