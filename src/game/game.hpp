#ifndef GAME_HPP
#define GAME_HPP

#include "game/logic.hpp"
#include "game/snake_body.hpp"

namespace Snake{

class Game {
  private:
    GameDifficulty game_difficulty;
    GameResult game_result;
    GameTable game_table;
    GameTable playable_area;
    Direction current_direction;
    Coordinates apple_position;
    SnakeBody *snake_body;
    uint32_t level;
    uint32_t score;

    void new_apple_position();

  public:
    Game(uint16_t table_height, uint16_t table_width, GameDifficulty game_difficulty, uint32_t level);

    GameResult update_game(Direction player_input);

    uint32_t calculate_points(uint32_t level, GameDifficulty difficulty) const;
    
    void win_game();

    GameDifficulty get_game_difficulty() const {
        return game_difficulty;
    }

    GameTable get_game_table() const {
        return game_table;
    }

    GameTable get_playable_area() const {
        return playable_area;
    }

    Coordinates get_apple_position() const {
        return apple_position;
    }

    SnakeBody *get_snake_body() const {
        return snake_body;
    }
    
    uint32_t get_score() const {
        return score;
    }

    GameResult get_game_result() const {
        return game_result;
    }
};
}
#endif