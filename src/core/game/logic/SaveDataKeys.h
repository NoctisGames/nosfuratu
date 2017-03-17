//
//  SaveDataKeys.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/9/17.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__SaveDataKeys__
#define __nosfuratu__SaveDataKeys__

inline std::string getKeyForNumGoldenCarrots()
{
    static std::string key = "v1_0_0_num_golden_carrots";
    
    return key;
}

inline std::string getKeyForJonUnlockedAbilitiesFlag()
{
    static std::string key = "v1_0_0_jon_unlocked_abilities_flag";
    
    return key;
}

inline std::string getKeyForViewedCutscenesFlag()
{
    static std::string key = "v1_0_0_viewed_cutscenes_flag";
    
    return key;
}

inline std::string getKeyForLevelScore(int world, int level)
{
    static std::vector<std::vector<std::string>> WORLDS_LEVELS_SCORES;
    static std::vector<std::string> world_1;
    
    if (WORLDS_LEVELS_SCORES.size() == 0)
    {
        world_1.push_back("v1_0_0_world_1_level_1_scores");
        world_1.push_back("v1_0_0_world_1_level_2_scores");
        world_1.push_back("v1_0_0_world_1_level_3_scores");
        world_1.push_back("v1_0_0_world_1_level_4_scores");
        world_1.push_back("v1_0_0_world_1_level_5_scores");
        world_1.push_back("v1_0_0_world_1_level_6_scores");
        world_1.push_back("v1_0_0_world_1_level_7_scores");
        world_1.push_back("v1_0_0_world_1_level_8_scores");
        world_1.push_back("v1_0_0_world_1_level_9_scores");
        world_1.push_back("v1_0_0_world_1_level_10_scores");
        world_1.push_back("v1_0_0_world_1_level_11_scores");
        world_1.push_back("v1_0_0_world_1_level_12_scores");
        world_1.push_back("v1_0_0_world_1_level_13_scores");
        world_1.push_back("v1_0_0_world_1_level_14_scores");
        world_1.push_back("v1_0_0_world_1_level_15_scores");
        world_1.push_back("v1_0_0_world_1_level_16_scores");
        world_1.push_back("v1_0_0_world_1_level_17_scores");
        world_1.push_back("v1_0_0_world_1_level_18_scores");
        world_1.push_back("v1_0_0_world_1_level_19_scores");
        world_1.push_back("v1_0_0_world_1_level_20_scores");
        world_1.push_back("v1_0_0_world_1_level_21_scores");
        
        WORLDS_LEVELS_SCORES.push_back(world_1);
    }
    
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    return WORLDS_LEVELS_SCORES[worldIndex][levelIndex];
}

inline std::string getKeyForLevelStats(int world, int level)
{
    static std::vector<std::vector<std::string>> WORLDS_LEVELS_STATS;
    static std::vector<std::string> world_1;
    
    if (WORLDS_LEVELS_STATS.size() == 0)
    {
        world_1.push_back("v1_0_0_world_1_level_1_stats");
        world_1.push_back("v1_0_0_world_1_level_2_stats");
        world_1.push_back("v1_0_0_world_1_level_3_stats");
        world_1.push_back("v1_0_0_world_1_level_4_stats");
        world_1.push_back("v1_0_0_world_1_level_5_stats");
        world_1.push_back("v1_0_0_world_1_level_6_stats");
        world_1.push_back("v1_0_0_world_1_level_7_stats");
        world_1.push_back("v1_0_0_world_1_level_8_stats");
        world_1.push_back("v1_0_0_world_1_level_9_stats");
        world_1.push_back("v1_0_0_world_1_level_10_stats");
        world_1.push_back("v1_0_0_world_1_level_11_stats");
        world_1.push_back("v1_0_0_world_1_level_12_stats");
        world_1.push_back("v1_0_0_world_1_level_13_stats");
        world_1.push_back("v1_0_0_world_1_level_14_stats");
        world_1.push_back("v1_0_0_world_1_level_15_stats");
        world_1.push_back("v1_0_0_world_1_level_16_stats");
        world_1.push_back("v1_0_0_world_1_level_17_stats");
        world_1.push_back("v1_0_0_world_1_level_18_stats");
        world_1.push_back("v1_0_0_world_1_level_19_stats");
        world_1.push_back("v1_0_0_world_1_level_20_stats");
        world_1.push_back("v1_0_0_world_1_level_21_stats");
        
        WORLDS_LEVELS_STATS.push_back(world_1);
    }
    
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    return WORLDS_LEVELS_STATS[worldIndex][levelIndex];
}

inline std::string getKeyForLevelOnlineScore(int world, int level)
{
    static std::vector<std::vector<std::string>> WORLDS_LEVELS_SCORES_ONLINE;
    static std::vector<std::string> world_1;
    
    if (WORLDS_LEVELS_SCORES_ONLINE.size() == 0)
    {
        world_1.push_back("v1_0_0_world_1_level_1_scores_online");
        world_1.push_back("v1_0_0_world_1_level_2_scores_online");
        world_1.push_back("v1_0_0_world_1_level_3_scores_online");
        world_1.push_back("v1_0_0_world_1_level_4_scores_online");
        world_1.push_back("v1_0_0_world_1_level_5_scores_online");
        world_1.push_back("v1_0_0_world_1_level_6_scores_online");
        world_1.push_back("v1_0_0_world_1_level_7_scores_online");
        world_1.push_back("v1_0_0_world_1_level_8_scores_online");
        world_1.push_back("v1_0_0_world_1_level_9_scores_online");
        world_1.push_back("v1_0_0_world_1_level_10_scores_online");
        world_1.push_back("v1_0_0_world_1_level_11_scores_online");
        world_1.push_back("v1_0_0_world_1_level_12_scores_online");
        world_1.push_back("v1_0_0_world_1_level_13_scores_online");
        world_1.push_back("v1_0_0_world_1_level_14_scores_online");
        world_1.push_back("v1_0_0_world_1_level_15_scores_online");
        world_1.push_back("v1_0_0_world_1_level_16_scores_online");
        world_1.push_back("v1_0_0_world_1_level_17_scores_online");
        world_1.push_back("v1_0_0_world_1_level_18_scores_online");
        world_1.push_back("v1_0_0_world_1_level_19_scores_online");
        world_1.push_back("v1_0_0_world_1_level_20_scores_online");
        world_1.push_back("v1_0_0_world_1_level_21_scores_online");
        
        WORLDS_LEVELS_SCORES_ONLINE.push_back(world_1);
    }
    
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    return WORLDS_LEVELS_SCORES_ONLINE[worldIndex][levelIndex];
}

#endif /* defined(__nosfuratu__SaveDataKeys__) */
