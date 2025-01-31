#ifndef LEADERBOARD_MANAGER_HPP
#define LEADERBOARD_MANAGER_HPP

#include "game/level_list.hpp"
#include "game/logic.hpp"

namespace Snake {

class LeaderboardManager {
private:
    LevelList level_list;
    
    const char* filename = "leaderboard.txt";
    const uint32_t maxEntriesPerDifficulty = 10;

    void sort_levels_list(GameDifficulty diff);

public:
    LeaderboardManager();
    void add_entry(uint32_t score, uint32_t level, GameDifficulty difficulty);
    LevelList get_entries(GameDifficulty difficulty);
    void load_entries();
    void save_entries();
};

} // namespace Snake

#endif