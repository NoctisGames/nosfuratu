#include "SaveData.h"

#include <vector>

using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace NosFURatu;

int SaveData::getNumGoldenCarrots()
{
    ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
    
    ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);
    
    auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;
    
    Platform::String^ key = getKeyForNumGoldenCarrots();
    
    bool hasValue = values->HasKey(key);
    
    if (hasValue)
    {
        return safe_cast<int>(values->Lookup(key));
    }
    
    return 0;
}

void SaveData::setNumGoldenCarrots(int numGoldenCarrots)
{
    ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
    
    ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);
    
    auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;
    
    Platform::String^ key = getKeyForNumGoldenCarrots();
    
    values->Insert(key, PropertyValue::CreateInt32(numGoldenCarrots));
}

int SaveData::getViewedCutscenesFlag()
{
    ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
    
    ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);
    
    auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;
    
    Platform::String^ key = getKeyForViewedCutscenesFlag();
    
    bool hasValue = values->HasKey(key);
    
    if (hasValue)
    {
        return safe_cast<int>(values->Lookup(key));
    }
    
    return 0;
}

void SaveData::setViewedCutscenesFlag(int viewedCutscenesFlag)
{
    ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
    
    ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);
    
    auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;
    
    Platform::String^ key = getKeyForViewedCutscenesFlag();
    
    values->Insert(key, PropertyValue::CreateInt32(viewedCutscenesFlag));
}

int SaveData::getJonUnlockedAbilitiesFlag()
{
    ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
    
    ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);
    
    auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;
    
    Platform::String^ key = getKeyForJonUnlockedAbilitiesFlag();
    
    bool hasValue = values->HasKey(key);
    
    if (hasValue)
    {
        return safe_cast<int>(values->Lookup(key));
    }
    
    return 0;
}

int SaveData::getLevelScore(int world, int level)
{
    ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
    
    ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);
    
    auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;
    
    Platform::String^ key = getKeyForLevelScore(world, level);
    
    bool hasValue = values->HasKey(key);
    
    if (hasValue)
    {
        return safe_cast<int>(values->Lookup(key));
    }
    
    return 0;
}

int SaveData::getLevelStatsFlag(int world, int level)
{
    ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
    
    ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);
    
    auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;
    
    Platform::String^ key = getKeyForLevelStats(world, level);
    
    bool hasValue = values->HasKey(key);
    
    if (hasValue)
    {
        return safe_cast<int>(values->Lookup(key));
    }
    
    return 0;
}

void SaveData::setLevelStatsFlag(int world, int level, int levelStatsFlag)
{
    ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
    
    ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);
    
    auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;
    
    {
        Platform::String^ key = getKeyForLevelStats(world, level);
        
        values->Insert(key, PropertyValue::CreateInt32(levelStatsFlag));
    }
}

void SaveData::setLevelComplete(int world, int level, int score, int levelStatsFlag, int jonUnlockedAbilitiesFlag)
{
    ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
    
    ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);
    
    auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;
    
    {
        Platform::String^ key = getKeyForLevelScore(world, level);
        
        values->Insert(key, PropertyValue::CreateInt32(score));
    }
    
    {
        Platform::String^ key = getKeyForLevelStats(world, level);
        
        values->Insert(key, PropertyValue::CreateInt32(levelStatsFlag));
    }
    
    {
        Platform::String^ key = getKeyForJonUnlockedAbilitiesFlag();
        
        values->Insert(key, PropertyValue::CreateInt32(jonUnlockedAbilitiesFlag));
    }
}

int SaveData::getScorePushedOnline(int world, int level)
{
    ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
    
    ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);
    
    auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;
    
    Platform::String^ key = getKeyForLevelOnlineScore(world, level);
    
    bool hasValue = values->HasKey(key);
    
    if (hasValue)
    {
        return safe_cast<int>(values->Lookup(key));
    }
    
    return 0;
}

void SaveData::setScorePushedOnline(int world, int level, int score)
{
    ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
    
    ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);
    
    auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;
    
    Platform::String^ key = getKeyForLevelOnlineScore(world, level);
    
    values->Insert(key, PropertyValue::CreateInt32(score));
}

Platform::String^ SaveData::getKeyForNumGoldenCarrots()
{
    static Platform::String^ key = "v0_9_9_num_golden_carrots";
    
    return key;
}

Platform::String^ SaveData::getKeyForJonUnlockedAbilitiesFlag()
{
    static Platform::String^ key = "v0_9_9_jon_unlocked_abilities_flag";
    
    return key;
}

Platform::String^ SaveData::getKeyForViewedCutscenesFlag()
{
    static Platform::String^ key = "v0_9_9_viewed_cutscenes_flag";
    
    return key;
}

Platform::String^ SaveData::getKeyForLevelScore(int world, int level)
{
    static std::vector<std::vector<Platform::String^>> WORLDS_LEVELS_SCORES;
    static std::vector<Platform::String^> world_1;
    static std::vector<Platform::String^> world_2;
    static std::vector<Platform::String^> world_3;
    static std::vector<Platform::String^> world_4;
    static std::vector<Platform::String^> world_5;
    
    if (WORLDS_LEVELS_SCORES.size() == 0)
    {
        world_1.push_back("v0_9_9_world_1_level_1_scores");
        world_1.push_back("v0_9_9_world_1_level_2_scores");
        world_1.push_back("v0_9_9_world_1_level_3_scores");
        world_1.push_back("v0_9_9_world_1_level_4_scores");
        world_1.push_back("v0_9_9_world_1_level_5_scores");
        world_1.push_back("v0_9_9_world_1_level_6_scores");
        world_1.push_back("v0_9_9_world_1_level_7_scores");
        world_1.push_back("v0_9_9_world_1_level_8_scores");
        world_1.push_back("v0_9_9_world_1_level_9_scores");
        world_1.push_back("v0_9_9_world_1_level_10_scores");
        world_1.push_back("v0_9_9_world_1_level_11_scores");
        world_1.push_back("v0_9_9_world_1_level_12_scores");
        world_1.push_back("v0_9_9_world_1_level_13_scores");
        world_1.push_back("v0_9_9_world_1_level_14_scores");
        world_1.push_back("v0_9_9_world_1_level_15_scores");
        world_1.push_back("v0_9_9_world_1_level_16_scores");
        world_1.push_back("v0_9_9_world_1_level_17_scores");
        world_1.push_back("v0_9_9_world_1_level_18_scores");
        world_1.push_back("v0_9_9_world_1_level_19_scores");
        world_1.push_back("v0_9_9_world_1_level_20_scores");
        world_1.push_back("v0_9_9_world_1_level_21_scores");
        
        WORLDS_LEVELS_SCORES.push_back(world_1);
        
        world_2.push_back("v0_9_9_world_2_level_1_scores");
        world_2.push_back("v0_9_9_world_2_level_2_scores");
        world_2.push_back("v0_9_9_world_2_level_3_scores");
        world_2.push_back("v0_9_9_world_2_level_4_scores");
        world_2.push_back("v0_9_9_world_2_level_5_scores");
        world_2.push_back("v0_9_9_world_2_level_6_scores");
        world_2.push_back("v0_9_9_world_2_level_7_scores");
        world_2.push_back("v0_9_9_world_2_level_8_scores");
        world_2.push_back("v0_9_9_world_2_level_9_scores");
        world_2.push_back("v0_9_9_world_2_level_10_scores");
        world_2.push_back("v0_9_9_world_2_level_11_scores");
        world_2.push_back("v0_9_9_world_2_level_12_scores");
        world_2.push_back("v0_9_9_world_2_level_13_scores");
        world_2.push_back("v0_9_9_world_2_level_14_scores");
        world_2.push_back("v0_9_9_world_2_level_15_scores");
        world_2.push_back("v0_9_9_world_2_level_16_scores");
        world_2.push_back("v0_9_9_world_2_level_17_scores");
        world_2.push_back("v0_9_9_world_2_level_18_scores");
        world_2.push_back("v0_9_9_world_2_level_19_scores");
        world_2.push_back("v0_9_9_world_2_level_20_scores");
        world_2.push_back("v0_9_9_world_2_level_21_scores");
        
        WORLDS_LEVELS_SCORES.push_back(world_2);
        
        world_3.push_back("v0_9_9_world_3_level_1_scores");
        world_3.push_back("v0_9_9_world_3_level_2_scores");
        world_3.push_back("v0_9_9_world_3_level_3_scores");
        world_3.push_back("v0_9_9_world_3_level_4_scores");
        world_3.push_back("v0_9_9_world_3_level_5_scores");
        world_3.push_back("v0_9_9_world_3_level_6_scores");
        world_3.push_back("v0_9_9_world_3_level_7_scores");
        world_3.push_back("v0_9_9_world_3_level_8_scores");
        world_3.push_back("v0_9_9_world_3_level_9_scores");
        world_3.push_back("v0_9_9_world_3_level_10_scores");
        world_3.push_back("v0_9_9_world_3_level_11_scores");
        world_3.push_back("v0_9_9_world_3_level_12_scores");
        world_3.push_back("v0_9_9_world_3_level_13_scores");
        world_3.push_back("v0_9_9_world_3_level_14_scores");
        world_3.push_back("v0_9_9_world_3_level_15_scores");
        world_3.push_back("v0_9_9_world_3_level_16_scores");
        world_3.push_back("v0_9_9_world_3_level_17_scores");
        world_3.push_back("v0_9_9_world_3_level_18_scores");
        world_3.push_back("v0_9_9_world_3_level_19_scores");
        world_3.push_back("v0_9_9_world_3_level_20_scores");
        world_3.push_back("v0_9_9_world_3_level_21_scores");
        
        WORLDS_LEVELS_SCORES.push_back(world_3);
        
        world_4.push_back("v0_9_9_world_4_level_1_scores");
        world_4.push_back("v0_9_9_world_4_level_2_scores");
        world_4.push_back("v0_9_9_world_4_level_3_scores");
        world_4.push_back("v0_9_9_world_4_level_4_scores");
        world_4.push_back("v0_9_9_world_4_level_5_scores");
        world_4.push_back("v0_9_9_world_4_level_6_scores");
        world_4.push_back("v0_9_9_world_4_level_7_scores");
        world_4.push_back("v0_9_9_world_4_level_8_scores");
        world_4.push_back("v0_9_9_world_4_level_9_scores");
        world_4.push_back("v0_9_9_world_4_level_10_scores");
        world_4.push_back("v0_9_9_world_4_level_11_scores");
        world_4.push_back("v0_9_9_world_4_level_12_scores");
        world_4.push_back("v0_9_9_world_4_level_13_scores");
        world_4.push_back("v0_9_9_world_4_level_14_scores");
        world_4.push_back("v0_9_9_world_4_level_15_scores");
        world_4.push_back("v0_9_9_world_4_level_16_scores");
        world_4.push_back("v0_9_9_world_4_level_17_scores");
        world_4.push_back("v0_9_9_world_4_level_18_scores");
        world_4.push_back("v0_9_9_world_4_level_19_scores");
        world_4.push_back("v0_9_9_world_4_level_20_scores");
        world_4.push_back("v0_9_9_world_4_level_21_scores");
        
        WORLDS_LEVELS_SCORES.push_back(world_4);
        
        world_5.push_back("v0_9_9_world_5_level_1_scores");
        world_5.push_back("v0_9_9_world_5_level_2_scores");
        world_5.push_back("v0_9_9_world_5_level_3_scores");
        world_5.push_back("v0_9_9_world_5_level_4_scores");
        world_5.push_back("v0_9_9_world_5_level_5_scores");
        world_5.push_back("v0_9_9_world_5_level_6_scores");
        world_5.push_back("v0_9_9_world_5_level_7_scores");
        world_5.push_back("v0_9_9_world_5_level_8_scores");
        world_5.push_back("v0_9_9_world_5_level_9_scores");
        world_5.push_back("v0_9_9_world_5_level_10_scores");
        world_5.push_back("v0_9_9_world_5_level_11_scores");
        world_5.push_back("v0_9_9_world_5_level_12_scores");
        world_5.push_back("v0_9_9_world_5_level_13_scores");
        world_5.push_back("v0_9_9_world_5_level_14_scores");
        world_5.push_back("v0_9_9_world_5_level_15_scores");
        world_5.push_back("v0_9_9_world_5_level_16_scores");
        world_5.push_back("v0_9_9_world_5_level_17_scores");
        world_5.push_back("v0_9_9_world_5_level_18_scores");
        world_5.push_back("v0_9_9_world_5_level_19_scores");
        world_5.push_back("v0_9_9_world_5_level_20_scores");
        world_5.push_back("v0_9_9_world_5_level_21_scores");
        
        WORLDS_LEVELS_SCORES.push_back(world_5);
    }
    
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    return WORLDS_LEVELS_SCORES[worldIndex][levelIndex];
}

Platform::String^ SaveData::getKeyForLevelStats(int world, int level)
{
	static std::vector<std::vector<Platform::String^>> WORLDS_LEVELS_STATS;
	static std::vector<Platform::String^> world_1;
	static std::vector<Platform::String^> world_2;
	static std::vector<Platform::String^> world_3;
	static std::vector<Platform::String^> world_4;
	static std::vector<Platform::String^> world_5;

	if (WORLDS_LEVELS_STATS.size() == 0)
	{
		world_1.push_back("v0_9_9_world_1_level_1_stats");
		world_1.push_back("v0_9_9_world_1_level_2_stats");
		world_1.push_back("v0_9_9_world_1_level_3_stats");
		world_1.push_back("v0_9_9_world_1_level_4_stats");
		world_1.push_back("v0_9_9_world_1_level_5_stats");
		world_1.push_back("v0_9_9_world_1_level_6_stats");
		world_1.push_back("v0_9_9_world_1_level_7_stats");
		world_1.push_back("v0_9_9_world_1_level_8_stats");
		world_1.push_back("v0_9_9_world_1_level_9_stats");
		world_1.push_back("v0_9_9_world_1_level_10_stats");
		world_1.push_back("v0_9_9_world_1_level_11_stats");
		world_1.push_back("v0_9_9_world_1_level_12_stats");
		world_1.push_back("v0_9_9_world_1_level_13_stats");
		world_1.push_back("v0_9_9_world_1_level_14_stats");
		world_1.push_back("v0_9_9_world_1_level_15_stats");
		world_1.push_back("v0_9_9_world_1_level_16_stats");
		world_1.push_back("v0_9_9_world_1_level_17_stats");
		world_1.push_back("v0_9_9_world_1_level_18_stats");
		world_1.push_back("v0_9_9_world_1_level_19_stats");
		world_1.push_back("v0_9_9_world_1_level_20_stats");
		world_1.push_back("v0_9_9_world_1_level_21_stats");

		WORLDS_LEVELS_STATS.push_back(world_1);

		world_2.push_back("v0_9_9_world_2_level_1_stats");
		world_2.push_back("v0_9_9_world_2_level_2_stats");
		world_2.push_back("v0_9_9_world_2_level_3_stats");
		world_2.push_back("v0_9_9_world_2_level_4_stats");
		world_2.push_back("v0_9_9_world_2_level_5_stats");
		world_2.push_back("v0_9_9_world_2_level_6_stats");
		world_2.push_back("v0_9_9_world_2_level_7_stats");
		world_2.push_back("v0_9_9_world_2_level_8_stats");
		world_2.push_back("v0_9_9_world_2_level_9_stats");
		world_2.push_back("v0_9_9_world_2_level_10_stats");
		world_2.push_back("v0_9_9_world_2_level_11_stats");
		world_2.push_back("v0_9_9_world_2_level_12_stats");
		world_2.push_back("v0_9_9_world_2_level_13_stats");
		world_2.push_back("v0_9_9_world_2_level_14_stats");
		world_2.push_back("v0_9_9_world_2_level_15_stats");
		world_2.push_back("v0_9_9_world_2_level_16_stats");
		world_2.push_back("v0_9_9_world_2_level_17_stats");
		world_2.push_back("v0_9_9_world_2_level_18_stats");
		world_2.push_back("v0_9_9_world_2_level_19_stats");
		world_2.push_back("v0_9_9_world_2_level_20_stats");
		world_2.push_back("v0_9_9_world_2_level_21_stats");

		WORLDS_LEVELS_STATS.push_back(world_2);

		world_3.push_back("v0_9_9_world_3_level_1_stats");
		world_3.push_back("v0_9_9_world_3_level_2_stats");
		world_3.push_back("v0_9_9_world_3_level_3_stats");
		world_3.push_back("v0_9_9_world_3_level_4_stats");
		world_3.push_back("v0_9_9_world_3_level_5_stats");
		world_3.push_back("v0_9_9_world_3_level_6_stats");
		world_3.push_back("v0_9_9_world_3_level_7_stats");
		world_3.push_back("v0_9_9_world_3_level_8_stats");
		world_3.push_back("v0_9_9_world_3_level_9_stats");
		world_3.push_back("v0_9_9_world_3_level_10_stats");
		world_3.push_back("v0_9_9_world_3_level_11_stats");
		world_3.push_back("v0_9_9_world_3_level_12_stats");
		world_3.push_back("v0_9_9_world_3_level_13_stats");
		world_3.push_back("v0_9_9_world_3_level_14_stats");
		world_3.push_back("v0_9_9_world_3_level_15_stats");
		world_3.push_back("v0_9_9_world_3_level_16_stats");
		world_3.push_back("v0_9_9_world_3_level_17_stats");
		world_3.push_back("v0_9_9_world_3_level_18_stats");
		world_3.push_back("v0_9_9_world_3_level_19_stats");
		world_3.push_back("v0_9_9_world_3_level_20_stats");
		world_3.push_back("v0_9_9_world_3_level_21_stats");

		WORLDS_LEVELS_STATS.push_back(world_3);

		world_4.push_back("v0_9_9_world_4_level_1_stats");
		world_4.push_back("v0_9_9_world_4_level_2_stats");
		world_4.push_back("v0_9_9_world_4_level_3_stats");
		world_4.push_back("v0_9_9_world_4_level_4_stats");
		world_4.push_back("v0_9_9_world_4_level_5_stats");
		world_4.push_back("v0_9_9_world_4_level_6_stats");
		world_4.push_back("v0_9_9_world_4_level_7_stats");
		world_4.push_back("v0_9_9_world_4_level_8_stats");
		world_4.push_back("v0_9_9_world_4_level_9_stats");
		world_4.push_back("v0_9_9_world_4_level_10_stats");
		world_4.push_back("v0_9_9_world_4_level_11_stats");
		world_4.push_back("v0_9_9_world_4_level_12_stats");
		world_4.push_back("v0_9_9_world_4_level_13_stats");
		world_4.push_back("v0_9_9_world_4_level_14_stats");
		world_4.push_back("v0_9_9_world_4_level_15_stats");
		world_4.push_back("v0_9_9_world_4_level_16_stats");
		world_4.push_back("v0_9_9_world_4_level_17_stats");
		world_4.push_back("v0_9_9_world_4_level_18_stats");
		world_4.push_back("v0_9_9_world_4_level_19_stats");
		world_4.push_back("v0_9_9_world_4_level_20_stats");
		world_4.push_back("v0_9_9_world_4_level_21_stats");

		WORLDS_LEVELS_STATS.push_back(world_4);

		world_5.push_back("v0_9_9_world_5_level_1_stats");
		world_5.push_back("v0_9_9_world_5_level_2_stats");
		world_5.push_back("v0_9_9_world_5_level_3_stats");
		world_5.push_back("v0_9_9_world_5_level_4_stats");
		world_5.push_back("v0_9_9_world_5_level_5_stats");
		world_5.push_back("v0_9_9_world_5_level_6_stats");
		world_5.push_back("v0_9_9_world_5_level_7_stats");
		world_5.push_back("v0_9_9_world_5_level_8_stats");
		world_5.push_back("v0_9_9_world_5_level_9_stats");
		world_5.push_back("v0_9_9_world_5_level_10_stats");
		world_5.push_back("v0_9_9_world_5_level_11_stats");
		world_5.push_back("v0_9_9_world_5_level_12_stats");
		world_5.push_back("v0_9_9_world_5_level_13_stats");
		world_5.push_back("v0_9_9_world_5_level_14_stats");
		world_5.push_back("v0_9_9_world_5_level_15_stats");
		world_5.push_back("v0_9_9_world_5_level_16_stats");
		world_5.push_back("v0_9_9_world_5_level_17_stats");
		world_5.push_back("v0_9_9_world_5_level_18_stats");
		world_5.push_back("v0_9_9_world_5_level_19_stats");
		world_5.push_back("v0_9_9_world_5_level_20_stats");
		world_5.push_back("v0_9_9_world_5_level_21_stats");

		WORLDS_LEVELS_STATS.push_back(world_5);
	}

	int worldIndex = world - 1;
	int levelIndex = level - 1;

	return WORLDS_LEVELS_STATS[worldIndex][levelIndex];
}

Platform::String^ SaveData::getKeyForLevelOnlineScore(int world, int level)
{
    static std::vector<std::vector<Platform::String^>> WORLDS_LEVELS_SCORES_ONLINE;
    static std::vector<Platform::String^> world_1;
    static std::vector<Platform::String^> world_2;
    static std::vector<Platform::String^> world_3;
    static std::vector<Platform::String^> world_4;
    static std::vector<Platform::String^> world_5;
    
    if (WORLDS_LEVELS_SCORES_ONLINE.size() == 0)
    {
        world_1.push_back("v0_9_9_world_1_level_1_scores_online");
        world_1.push_back("v0_9_9_world_1_level_2_scores_online");
        world_1.push_back("v0_9_9_world_1_level_3_scores_online");
        world_1.push_back("v0_9_9_world_1_level_4_scores_online");
        world_1.push_back("v0_9_9_world_1_level_5_scores_online");
        world_1.push_back("v0_9_9_world_1_level_6_scores_online");
        world_1.push_back("v0_9_9_world_1_level_7_scores_online");
        world_1.push_back("v0_9_9_world_1_level_8_scores_online");
        world_1.push_back("v0_9_9_world_1_level_9_scores_online");
        world_1.push_back("v0_9_9_world_1_level_10_scores_online");
        world_1.push_back("v0_9_9_world_1_level_11_scores_online");
        world_1.push_back("v0_9_9_world_1_level_12_scores_online");
        world_1.push_back("v0_9_9_world_1_level_13_scores_online");
        world_1.push_back("v0_9_9_world_1_level_14_scores_online");
        world_1.push_back("v0_9_9_world_1_level_15_scores_online");
        world_1.push_back("v0_9_9_world_1_level_16_scores_online");
        world_1.push_back("v0_9_9_world_1_level_17_scores_online");
        world_1.push_back("v0_9_9_world_1_level_18_scores_online");
        world_1.push_back("v0_9_9_world_1_level_19_scores_online");
        world_1.push_back("v0_9_9_world_1_level_20_scores_online");
        world_1.push_back("v0_9_9_world_1_level_21_scores_online");
        
        WORLDS_LEVELS_SCORES_ONLINE.push_back(world_1);
        
        world_2.push_back("v0_9_9_world_2_level_1_scores_online");
        world_2.push_back("v0_9_9_world_2_level_2_scores_online");
        world_2.push_back("v0_9_9_world_2_level_3_scores_online");
        world_2.push_back("v0_9_9_world_2_level_4_scores_online");
        world_2.push_back("v0_9_9_world_2_level_5_scores_online");
        world_2.push_back("v0_9_9_world_2_level_6_scores_online");
        world_2.push_back("v0_9_9_world_2_level_7_scores_online");
        world_2.push_back("v0_9_9_world_2_level_8_scores_online");
        world_2.push_back("v0_9_9_world_2_level_9_scores_online");
        world_2.push_back("v0_9_9_world_2_level_10_scores_online");
        world_2.push_back("v0_9_9_world_2_level_11_scores_online");
        world_2.push_back("v0_9_9_world_2_level_12_scores_online");
        world_2.push_back("v0_9_9_world_2_level_13_scores_online");
        world_2.push_back("v0_9_9_world_2_level_14_scores_online");
        world_2.push_back("v0_9_9_world_2_level_15_scores_online");
        world_2.push_back("v0_9_9_world_2_level_16_scores_online");
        world_2.push_back("v0_9_9_world_2_level_17_scores_online");
        world_2.push_back("v0_9_9_world_2_level_18_scores_online");
        world_2.push_back("v0_9_9_world_2_level_19_scores_online");
        world_2.push_back("v0_9_9_world_2_level_20_scores_online");
        world_2.push_back("v0_9_9_world_2_level_21_scores_online");
        
        WORLDS_LEVELS_SCORES_ONLINE.push_back(world_2);
        
        world_3.push_back("v0_9_9_world_3_level_1_scores_online");
        world_3.push_back("v0_9_9_world_3_level_2_scores_online");
        world_3.push_back("v0_9_9_world_3_level_3_scores_online");
        world_3.push_back("v0_9_9_world_3_level_4_scores_online");
        world_3.push_back("v0_9_9_world_3_level_5_scores_online");
        world_3.push_back("v0_9_9_world_3_level_6_scores_online");
        world_3.push_back("v0_9_9_world_3_level_7_scores_online");
        world_3.push_back("v0_9_9_world_3_level_8_scores_online");
        world_3.push_back("v0_9_9_world_3_level_9_scores_online");
        world_3.push_back("v0_9_9_world_3_level_10_scores_online");
        world_3.push_back("v0_9_9_world_3_level_11_scores_online");
        world_3.push_back("v0_9_9_world_3_level_12_scores_online");
        world_3.push_back("v0_9_9_world_3_level_13_scores_online");
        world_3.push_back("v0_9_9_world_3_level_14_scores_online");
        world_3.push_back("v0_9_9_world_3_level_15_scores_online");
        world_3.push_back("v0_9_9_world_3_level_16_scores_online");
        world_3.push_back("v0_9_9_world_3_level_17_scores_online");
        world_3.push_back("v0_9_9_world_3_level_18_scores_online");
        world_3.push_back("v0_9_9_world_3_level_19_scores_online");
        world_3.push_back("v0_9_9_world_3_level_20_scores_online");
        world_3.push_back("v0_9_9_world_3_level_21_scores_online");
        
        WORLDS_LEVELS_SCORES_ONLINE.push_back(world_3);
        
        world_4.push_back("v0_9_9_world_4_level_1_scores_online");
        world_4.push_back("v0_9_9_world_4_level_2_scores_online");
        world_4.push_back("v0_9_9_world_4_level_3_scores_online");
        world_4.push_back("v0_9_9_world_4_level_4_scores_online");
        world_4.push_back("v0_9_9_world_4_level_5_scores_online");
        world_4.push_back("v0_9_9_world_4_level_6_scores_online");
        world_4.push_back("v0_9_9_world_4_level_7_scores_online");
        world_4.push_back("v0_9_9_world_4_level_8_scores_online");
        world_4.push_back("v0_9_9_world_4_level_9_scores_online");
        world_4.push_back("v0_9_9_world_4_level_10_scores_online");
        world_4.push_back("v0_9_9_world_4_level_11_scores_online");
        world_4.push_back("v0_9_9_world_4_level_12_scores_online");
        world_4.push_back("v0_9_9_world_4_level_13_scores_online");
        world_4.push_back("v0_9_9_world_4_level_14_scores_online");
        world_4.push_back("v0_9_9_world_4_level_15_scores_online");
        world_4.push_back("v0_9_9_world_4_level_16_scores_online");
        world_4.push_back("v0_9_9_world_4_level_17_scores_online");
        world_4.push_back("v0_9_9_world_4_level_18_scores_online");
        world_4.push_back("v0_9_9_world_4_level_19_scores_online");
        world_4.push_back("v0_9_9_world_4_level_20_scores_online");
        world_4.push_back("v0_9_9_world_4_level_21_scores_online");
        
        WORLDS_LEVELS_SCORES_ONLINE.push_back(world_4);
        
        world_5.push_back("v0_9_9_world_5_level_1_scores_online");
        world_5.push_back("v0_9_9_world_5_level_2_scores_online");
        world_5.push_back("v0_9_9_world_5_level_3_scores_online");
        world_5.push_back("v0_9_9_world_5_level_4_scores_online");
        world_5.push_back("v0_9_9_world_5_level_5_scores_online");
        world_5.push_back("v0_9_9_world_5_level_6_scores_online");
        world_5.push_back("v0_9_9_world_5_level_7_scores_online");
        world_5.push_back("v0_9_9_world_5_level_8_scores_online");
        world_5.push_back("v0_9_9_world_5_level_9_scores_online");
        world_5.push_back("v0_9_9_world_5_level_10_scores_online");
        world_5.push_back("v0_9_9_world_5_level_11_scores_online");
        world_5.push_back("v0_9_9_world_5_level_12_scores_online");
        world_5.push_back("v0_9_9_world_5_level_13_scores_online");
        world_5.push_back("v0_9_9_world_5_level_14_scores_online");
        world_5.push_back("v0_9_9_world_5_level_15_scores_online");
        world_5.push_back("v0_9_9_world_5_level_16_scores_online");
        world_5.push_back("v0_9_9_world_5_level_17_scores_online");
        world_5.push_back("v0_9_9_world_5_level_18_scores_online");
        world_5.push_back("v0_9_9_world_5_level_19_scores_online");
        world_5.push_back("v0_9_9_world_5_level_20_scores_online");
        world_5.push_back("v0_9_9_world_5_level_21_scores_online");
        
        WORLDS_LEVELS_SCORES_ONLINE.push_back(world_5);
    }
    
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    return WORLDS_LEVELS_SCORES_ONLINE[worldIndex][levelIndex];
}
