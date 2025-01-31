#ifndef LEVEL_LIST_HPP
#define LEVEL_LIST_HPP

#include "game/logic.hpp"
#include <stddef.h>

namespace Snake {

struct LevelListElement {
    LevelInfo info;
    LevelListElement *previous;
    LevelListElement *next;
};

class LevelList {
  private:
    size_t element_count;
    LevelListElement *head;
    LevelListElement *selected; // keeps track of current level

  public:
    LevelList();
    LevelList(const char* file_path);
    ~LevelList();

    void add_element(LevelInfo level_info);

    LevelListElement *get_element_at(size_t index);

    LevelListElement *remove_element_at(int index);

    size_t get_element_count();

    LevelListElement *next_level();
    LevelListElement *previous_level();

    void save_as_file(const char* file_path);
};
} // namespace Snake

#endif // LIST_HPP
