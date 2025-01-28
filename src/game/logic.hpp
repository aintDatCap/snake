#ifndef LOGIC_HPP
#define LOGIC_HPP

#include "utils/queue.hpp"
#include <cstdint>

namespace Snake {

typedef struct {
    uint16_t height;
    uint16_t width;
} GameTable;

typedef struct {
    uint16_t x;
    uint16_t y;
} Coordinates;

bool coordinates_are_equal(Coordinates a, Coordinates b);

typedef enum : int8_t {
    DIRECTION_UP = 1,
    DIRECTION_DOWN = ~1,
    DIRECTION_LEFT = 2,
    DIRECTION_RIGHT = ~2,
    DIRECTION_NONE = 0
} Direction;

typedef enum {
    GAME_UNFINISHED,
    GAME_WON,
    GAME_LOST
} GameResult;

typedef enum {
    DIFFICULTY_EASY = 1,
    DIFFICULTY_NORMAL = 5,
    DIFFICULTY_HARD = 8, // random values
} GameDifficulty;

typedef struct {
    uint32_t high_score;
} LevelInfo;

class Game {
  private:
    GameDifficulty game_difficulty;
    GameResult game_result;
    GameTable game_table;
    GameTable playable_area;
    Direction current_direction;
    Coordinates snake_head_position;
    Coordinates apple_position;
    Queue<Coordinates> *snake_body;
    uint32_t level;
    uint32_t speed;
    uint32_t score;

    void new_apple_position();

  public:
    Game(uint16_t table_height, uint16_t table_width, GameDifficulty game_difficulty, uint32_t level);

    GameResult update_game(Direction player_input);

    uint32_t calculate_points(uint32_t level, GameDifficulty difficulty) const;

    static GameTable get_playable_dimensions(GameDifficulty difficulty);

    void set_speed(uint32_t level);
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

    Coordinates get_snake_head_position() const {
        return snake_head_position;
    }

    Coordinates get_apple_position() const {
        return apple_position;
    }

    Queue<Coordinates> *get_snake_body() const {
        return snake_body;
    }

    void set_level(uint32_t new_level) {
        level = new_level;
    }

    uint32_t get_speed() const {
        return speed;
    }

    uint32_t get_score() const {
        return score;
    }

    GameResult get_game_result() const {
        return game_result;
    }
};

} // namespace Snake

#endif