#ifndef SNAKE_BODY_CPP
#define SNAKE_BODY_CPP

#include "game/snake_body.hpp"
#include "game/logic.hpp"
#include <cstddef>
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
    new_elem->next = this->head;

    this->head = new_elem;
}

SnakePart *SnakeBody::dequeue() {
    if(!this->head){
        return nullptr;
    }

    if(!this->head->next) {
        SnakePart *elem = this->head;
        this->head = nullptr;
        return elem;
    }

    SnakePart* current = this->head;
    while (current->next->next) {
        current = current->next;
    }

    SnakePart *elem = current->next;
    current->next = nullptr;
    return elem;

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
SnakePart *SnakeBody::get_element_at(size_t index) {
    if (!head) {
        return nullptr; // Empty queue
    }

    SnakePart *current = head;
    size_t current_index = 0;

    while (current) {
        if (current_index == index) {
            return current; // Return element at specified index
        }
        current_index++;
        current = current->next;
    }
    return nullptr; // Return nullptr if index is out of bounds
}
SnakePart *SnakeBody::get_head() {
    return this->head;
}

} // namespace Snake

#endif