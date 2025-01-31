#ifndef SNAKE_BODY_HPP
#define SNAKE_BODY_HPP

#include "game/logic.hpp"
#include <stddef.h>

namespace Snake {

struct SnakePart {
    Coordinates position;
    SnakePart *next;
};

class SnakeBody {
  private:
    SnakePart *head;

  public:
    SnakeBody(Coordinates head_position);
    ~SnakeBody();

    void enqueue(Snake::Coordinates position);
    SnakePart *dequeue();

    // Method to get an element at a specific index in the queue
    SnakePart *get_element_at(size_t index);
    SnakePart *get_head();
    size_t size();
};
} // namespace Snake
#endif