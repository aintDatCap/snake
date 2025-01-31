#ifndef LEVELS_LIST_HPP
#define LEVELS_LIST_HPP

#include "game/logic.hpp"
#include <stddef.h>

namespace Snake {

struct LevelsListElement {
    LevelInfo info;
    LevelsListElement *previous;
    LevelsListElement *next;
};

class LevelsList {
  private:
    size_t element_count;
    LevelsListElement *head;
    LevelsListElement *selected; // keeps track of current level

  public:
    LevelsList();
    ~LevelsList();

    void add_element(LevelInfo level_info);

    LevelsListElement *get_element_at(size_t index);

    LevelsListElement *remove_element_at(int index);

    size_t get_element_count();

    LevelsListElement *next_level();
    LevelsListElement *previous_level();
};
} // namespace Snake

#endif // LIST_HPP
