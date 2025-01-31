#ifndef LEADERBOARD_MANAGER_CPP
#define LEADERBOARD_MANAGER_CPP

#include "leaderboard_manager.hpp"
#include "game/level_list.hpp"
#include "logic.hpp"

namespace Snake {

LeaderboardManager::LeaderboardManager() {
    load_entries();
}

void LeaderboardManager::add_entry(uint32_t score, uint32_t level, GameDifficulty difficulty) {
    // Load current entries
    load_entries();
    
    // Add new entry
    level_list.add_element(LevelInfo(score, level, difficulty));
    
    // Sort entries for this difficulty and remove extra entries
    sort_levels_list(difficulty);
    
    // Remove excess entries for this difficulty
    uint32_t count = 0;
    for(uint32_t i = 0; i < level_list.get_element_count();) {
        if(level_list.get_element_at(i)->info.difficulty == difficulty) {
            if(++count > maxEntriesPerDifficulty) {
                level_list.remove_element_at(i);
            } else {
                i++;
            }
        } else {
            i++;
        }
    }
    
    save_entries();
}

void LeaderboardManager::sort_levels_list(GameDifficulty diff) {
    // bubble sort
    bool swapped;
    for(uint32_t i = 0; i < level_list.get_element_count()-1; i++) {
        swapped = false;
        for(uint32_t j = 0; j < level_list.get_element_count()-i-1; j++) {
            LevelInfo current = level_list.get_element_at(j)->info;
            LevelInfo next = level_list.get_element_at(j+1)->info;
            
            if(current.difficulty == diff && next.difficulty == diff) {
                if(current.high_score < next.high_score) {
                    LevelInfo temp = current;
                    current = next;
                    next = temp;
                    swapped = true;
                }
            }
        }
        if(!swapped) break;
    }
}

LevelList LeaderboardManager::get_entries(GameDifficulty difficulty) {
    LevelList result;
    for(uint32_t i = 0; i < level_list.get_element_count(); i++) {
        LevelListElement* element = level_list.get_element_at(i);
        if(element->info.difficulty == difficulty) {
            result.add_element(element->info);
        }
    }
    return result;
}

void LeaderboardManager::load_entries() {
    level_list = LevelList(filename);
}

void LeaderboardManager::save_entries() {
    level_list.save_as_file(filename);
}

// std::string LeaderboardManager::difficulty_to_string(GameDifficulty diff) const {
//     switch(diff) {
//         case DIFFICULTY_EASY: return "Easy";
//         case DIFFICULTY_NORMAL: return "Normal";
//         case DIFFICULTY_HARD: return "Hard";
//         default: return "Unknown";
//     }
// }

// GameDifficulty LeaderboardManager::string_to_difficulty(const char* s) const {
//     if(strcmp(s, "Easy") == 0) return DIFFICULTY_EASY;
//     if(strcmp(s, "Normal") == 0) return DIFFICULTY_NORMAL;
//     if(strcmp(s, "Hard") == 0) return DIFFICULTY_HARD;
//     return DIFFICULTY_EASY;
// }

} // namespace Snake
#endif