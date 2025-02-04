#ifndef LEVELS_LIST_CPP
#define LEVELS_LIST_CPP

#include "game/level_list.hpp"
#include "game/logic.hpp"
#include <cstddef>
#include <cstdio>

namespace Snake {

LevelList::LevelList() {
    this->head = nullptr;
    this->selected = nullptr;
}

LevelList::~LevelList() {
    while (head) {
        remove_element_at(0); // Remove elements one by one
    }
}

void LevelList::add_element(LevelInfo level_info) {
    LevelListElement *new_element = new LevelListElement;
    new_element->info = level_info;
    if (!head) { // if head still empty
        this->head = new_element;
        this->head->next = nullptr;
        this->head->previous = nullptr;

        this->selected = head;
        return;
    }

    LevelListElement *current = head;

    while (current->next) { // go to the end of the list...
        current = current->next;
    }
    current->next = new_element;
    new_element->previous = current;
}

LevelListElement *LevelList::get_element_at(size_t index) {
    if (!head) { // empty list
        return nullptr;
    }

    LevelListElement *current = head;
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

LevelListElement *LevelList::remove_element_at(int index) {
    if (!this->head) {
        return nullptr;
    }
    LevelListElement *current = this->head;
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

    return current;
}

size_t LevelList::get_element_count() {
    size_t count = 0;
    LevelListElement *current = this->head;
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

size_t LevelList::get_element_count(GameDifficulty difficulty) {
    size_t count = 0;
    LevelListElement *current = this->head;
    while (current) {
        if (current->info.difficulty == difficulty) {
            count++;
        }

        current = current->next;
    }
    return count;
}

LevelListElement *LevelList::next_level() {
    if (!head || !selected->next) {
        return nullptr;
    }
    this->selected = this->selected->next;
    return this->selected;
}

LevelListElement *LevelList::get_current() {
    return this->selected;
}

bool LevelList::set_current_level(GameDifficulty difficulty, size_t index) {
    LevelListElement *current = this->head;
    while (current) {
        if (current->info.difficulty == difficulty && current->info.id == index) {
            this->selected = current;
            return true;
        }
        current = current->next;
    }
    return false;
}

LevelList *LevelList::from_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        return nullptr;
    }

    LevelList *level_list = new LevelList();
    LevelInfo *level_info = new LevelInfo;
    while (std::fread(level_info, sizeof(LevelInfo), 1, file)) {
        // I PRAY THAT THIS IS GOING TO WORK WHEN
        // I'M GOING TO SHOW IT TO THE COMMISSION
        level_list->add_element(*level_info);
    }

    fclose(file);

    return level_list;
}

void LevelList::save_as_file(const char *file_path) {
    FILE *file = std::fopen(file_path, "w");
    if (file != NULL) {

        for (size_t i = 0; i < this->get_element_count(); i++) {
            std::fwrite(&this->get_element_at(i)->info, sizeof(LevelInfo), 1, file);
        }
        fclose(file);
    }
}

} // namespace Snake

#endif