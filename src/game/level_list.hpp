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
    LevelListElement *head;
    LevelListElement *selected; // keeps track of current level

  public:
    LevelList();
    ~LevelList();

    void add_element(LevelInfo level_info);

    LevelListElement *get_element_at(size_t index);

    LevelListElement *remove_element_at(int index);

    // Returns the number of elements inside of the list
    size_t get_element_count();

    // Returns the number of elements inside of the list with a certain difficulty
    size_t get_element_count(GameDifficulty difficulty);

    // Goes to the next level of the current difficulty and returns it
    LevelListElement *next_level();

    LevelListElement *get_current();

    // Searches for the level with a certain difficulty and index
    // and sets it as the current selected level
    // Returns true if found
    // Returns false otherwise
    bool set_current_level(GameDifficulty difficulty, size_t index);

    static LevelList *from_file(const char *file_path);
    void save_as_file(const char *file_path);
};
} // namespace Snake

#endif // LIST_HPP
