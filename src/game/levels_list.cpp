#ifndef LEVELS_LIST_CPP
#define LEVELS_LIST_CPP

#include "game/levels_list.hpp"

namespace Snake {

LevelsList::LevelsList() {
    this->head = nullptr;
    this->selected = nullptr;
    this->element_count = 0;
}
LevelsList::~LevelsList() {
    while (head) {
        remove_element_at(0); // Remove elements one by one
    }
}

void LevelsList::add_element(LevelInfo level_info) {
    LevelsListElement *new_element = new LevelsListElement;
    new_element->info = level_info;
    if (!head) { // if head still empty
        this->head = new_element;
        this->head->next = nullptr;
        this->head->previous = nullptr;

        this->selected = head;
        this->element_count++;
        return;
    }

    LevelsListElement *current = head;

    while (current->next) { // go to the end of the list...
        current = current->next;
    }
    current->next = new_element;
    new_element->previous = current;

    element_count++;
}

LevelsListElement *LevelsList::get_element_at(size_t index) {
    if (!head) { // empty list
        return nullptr;
    }

    LevelsListElement *current = head;
    size_t i = 0;
    while (i < index) {
        if (!current->next) { // if could not reach till the end...
            return nullptr;
        }
        current = current->next;
        i++;
    }
    return current;
}

LevelsListElement *LevelsList::remove_element_at(int index) {
    if (!this->head) {
        return nullptr;
    }
    LevelsListElement *current = this->head;
    int i = 0;
    while (i < index) {
        if (!current->next) {
            return nullptr;
        }
        current = current->next;
        i++;
    }

    if (current == this->head) { // if head removed...
        if (this->selected == this->head) {
            this->selected = this->head->next;
        }
        this->head = this->head->next;
        return nullptr;
    }

    // if got to the right element...
    current->previous->next = current->next;
    current->next->previous = current->previous;

    this->element_count--;
    return current;
}

size_t LevelsList::get_element_count() {
    return this->element_count;
}

LevelsListElement *LevelsList::next_level() {
    if (!head || !selected->next) {
        return nullptr;
    }
    this->selected = this->selected->next;
    return this->selected;
}

LevelsListElement *LevelsList::previous_level() {
    if (!head || !selected->previous) {
        return nullptr;
    }
    this->selected = this->selected->previous;
    return this->selected;
}

} // namespace Snake

#endif