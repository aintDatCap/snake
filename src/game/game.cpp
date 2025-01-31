#ifndef GAME_CPP
#define GAME_CPP

#include "game/game.hpp"
#include <stdexcept>
#include <stdlib.h>

namespace Snake {

Game::Game(uint16_t table_height, uint16_t table_width, GameDifficulty game_difficulty, uint32_t level) {
    this->game_difficulty = game_difficulty;
    this->game_result = GAME_UNFINISHED;

    this->game_table.height = table_height;
    this->game_table.width = table_width;

    this->current_direction = DIRECTION_UP;

    this->playable_area = get_playable_dimensions(game_difficulty);

    this->snake_head_position.x = playable_area.width / 2;  // Center X
    this->snake_head_position.y = playable_area.height / 2; // Center Y

    this->snake_body = new Queue<Coordinates>();

    uint16_t remaining_snake_body_size = SNAKE_MINIMUM_BODY_SIZE + game_difficulty;
    // Avoid asking yourself why it works
    while (remaining_snake_body_size) {
        Coordinates coords;
        if (remaining_snake_body_size + this->snake_head_position.y >= this->playable_area.height - 2) {
            coords.y = this->playable_area.height - 2;
            coords.x =
                this->snake_head_position.x + remaining_snake_body_size - (coords.y - this->snake_head_position.y);

        } else {
            coords.x = this->snake_head_position.x;
            coords.y = this->snake_head_position.y + remaining_snake_body_size;
        }
        remaining_snake_body_size--;
        this->snake_body->enqueue(coords);
    }
    this->score = 0;
    this->level = level;
    this->new_apple_position();
}

void Game::new_apple_position() {
    bool valid_position = false;
    while (!valid_position) {
        // Generate random coordinates within the bounds of the playable area,
        // but avoid the borders (first and last rows/columns)
        this->apple_position.x = rand() % (this->playable_area.width - 2) + 1;  // Avoid left and right borders
        this->apple_position.y = rand() % (this->playable_area.height - 2) + 1; // Avoid top and bottom borders

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

uint32_t Game::calculate_points(uint32_t level, GameDifficulty difficulty) const {
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

GameResult Game::update_game(Direction player_input) {
    if (this->game_result != GAME_UNFINISHED) {
        return this->game_result;
    }

    if (coordinates_are_equal(this->snake_head_position, this->apple_position)) {
        // Increase score and create a new apple
        this->score += this->calculate_points(this->level, this->game_difficulty);
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
                game_result = GAME_LOST;
                return GAME_LOST;
            }
            break;
        }
        case DIRECTION_DOWN: {
            // add 1 to y
            snake_head_position.y++;

            if (snake_head_position.y == playable_area.height - 1) { // -1 because of the border
                game_result = GAME_LOST;
                return GAME_LOST;
            }
            break;
        }
        case DIRECTION_LEFT: {
            // remove 1 to x
            snake_head_position.x--;
            if (snake_head_position.x == 0) {
                game_result = GAME_LOST;
                return GAME_LOST;
            }
            break;
        }
        case DIRECTION_RIGHT: {
            // add 1 to x
            snake_head_position.x++;

            if (snake_head_position.x == playable_area.width - 1) {
                game_result = GAME_LOST;
                return GAME_LOST;
            }
            break;
        }
        default: {
            throw std::invalid_argument("player_input should assume only values defined by the 'Direction' enum");
            break;
        }
    }

    // if the head collides with the body, then the game is lost
    for (size_t i = 0; i < this->snake_body->size(); i++) {
        if (coordinates_are_equal(this->snake_head_position, this->snake_body->get_element_at(i)->value)) {
            game_result = GAME_LOST;
            return GAME_LOST;
        }
    }

    return GAME_UNFINISHED;
}

void Game::win_game() {
    if (this->game_result == GAME_UNFINISHED) {
        this->game_result = GAME_WON;
    }
}
} // namespace Snake

#endif