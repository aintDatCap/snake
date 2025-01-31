#ifndef SNAKE_BODY_CPP
#define SNAKE_BODY_CPP

#include "game/snake_body.hpp"
namespace Snake {

SnakeBody::SnakeBody(Coordinates head_position) {
    head = new SnakePart;
    head->position = head_position;
    head->next = nullptr;
}

SnakeBody::~SnakeBody() {
    while (head) {
        SnakePart *current = head;
        head = head->next;
        delete current;
    }
}

void SnakeBody::enqueue(Snake::Coordinates position) {
    SnakePart *new_elem = new SnakePart;
    new_elem->position = position;
    new_elem->next = nullptr;

    if (!head) {
        head = new_elem;
        return;
    }
    // Insert at the end of the queue
    SnakePart *current = head;
    while (current->next) {
        current = current->next;
    }
    current->next = new_elem;
}

SnakePart *SnakeBody::dequeue() {
    if (!head) {
        return nullptr; // Queue is empty
    }

    SnakePart *elem = head;
    head = head->next; // Move head to the next element
    return elem;       // Return the dequeued element
}

size_t SnakeBody::size() {
    size_t count = 0;
    SnakePart *current = head;
    while (current) {
        ++count;
        current = current->next;
    }
    return count;
}

// Method to get an element at a specific index in the queue
SnakePart *SnakeBody::get_element_at(int index) {
    if (!head) {
        return nullptr; // Empty queue
    }

    SnakePart *current = head;
    int current_index = 0;

    while (current) {
        if (current_index == index) {
            return current; // Return element at specified index
        }
        current_index++;
        current = current->next;
    }
    return nullptr; // Return nullptr if index is out of bounds
}

} // namespace Snake

#endif