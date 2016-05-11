#include "SaveData.h"

#include <vector>

using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace NosFURatu;

int SaveData::getLevelStats(int world, int level)
{
	ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;

	ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);

	auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;

	Platform::String^ key = getKeyForLevel(world, level);

	bool hasValue = values->HasKey(key);
	
	if (hasValue)
	{
        return safe_cast<int>(values->Lookup(key));
	}

	return 0;
}

void SaveData::setLevelComplete(int world, int level, int goldenCarrotsFlag)
{
	ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;

	ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);

	auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;

	Platform::String^ key = getKeyForLevel(world, level);

	values->Insert(key, PropertyValue::CreateInt32(goldenCarrotsFlag));
}

Platform::String^ SaveData::getKeyForLevel(int world, int level)
{
	static std::vector<std::vector<Platform::String^>> WORLDS_LEVELS_STATS;
	static std::vector<Platform::String^> world_1;
	static std::vector<Platform::String^> world_2;
	static std::vector<Platform::String^> world_3;
	static std::vector<Platform::String^> world_4;
	static std::vector<Platform::String^> world_5;

	if (WORLDS_LEVELS_STATS.size() == 0)
	{
		world_1.push_back("world_1_level_1_stats");
		world_1.push_back("world_1_level_2_stats");
		world_1.push_back("world_1_level_3_stats");
		world_1.push_back("world_1_level_4_stats");
		world_1.push_back("world_1_level_5_stats");
		world_1.push_back("world_1_level_6_stats");
		world_1.push_back("world_1_level_7_stats");
		world_1.push_back("world_1_level_8_stats");
		world_1.push_back("world_1_level_9_stats");
		world_1.push_back("world_1_level_10_stats");
		world_1.push_back("world_1_level_11_stats");
		world_1.push_back("world_1_level_12_stats");
		world_1.push_back("world_1_level_13_stats");
		world_1.push_back("world_1_level_14_stats");
		world_1.push_back("world_1_level_15_stats");
		world_1.push_back("world_1_level_16_stats");
		world_1.push_back("world_1_level_17_stats");
		world_1.push_back("world_1_level_18_stats");
		world_1.push_back("world_1_level_19_stats");
		world_1.push_back("world_1_level_20_stats");
		world_1.push_back("world_1_level_21_stats");

		WORLDS_LEVELS_STATS.push_back(world_1);

		world_2.push_back("world_2_level_1_stats");
		world_2.push_back("world_2_level_2_stats");
		world_2.push_back("world_2_level_3_stats");
		world_2.push_back("world_2_level_4_stats");
		world_2.push_back("world_2_level_5_stats");
		world_2.push_back("world_2_level_6_stats");
		world_2.push_back("world_2_level_7_stats");
		world_2.push_back("world_2_level_8_stats");
		world_2.push_back("world_2_level_9_stats");
		world_2.push_back("world_2_level_10_stats");
		world_2.push_back("world_2_level_11_stats");
		world_2.push_back("world_2_level_12_stats");
		world_2.push_back("world_2_level_13_stats");
		world_2.push_back("world_2_level_14_stats");
		world_2.push_back("world_2_level_15_stats");
		world_2.push_back("world_2_level_16_stats");
		world_2.push_back("world_2_level_17_stats");
		world_2.push_back("world_2_level_18_stats");
		world_2.push_back("world_2_level_19_stats");
		world_2.push_back("world_2_level_20_stats");
		world_2.push_back("world_2_level_21_stats");

		WORLDS_LEVELS_STATS.push_back(world_2);

		world_3.push_back("world_3_level_1_stats");
		world_3.push_back("world_3_level_2_stats");
		world_3.push_back("world_3_level_3_stats");
		world_3.push_back("world_3_level_4_stats");
		world_3.push_back("world_3_level_5_stats");
		world_3.push_back("world_3_level_6_stats");
		world_3.push_back("world_3_level_7_stats");
		world_3.push_back("world_3_level_8_stats");
		world_3.push_back("world_3_level_9_stats");
		world_3.push_back("world_3_level_10_stats");
		world_3.push_back("world_3_level_11_stats");
		world_3.push_back("world_3_level_12_stats");
		world_3.push_back("world_3_level_13_stats");
		world_3.push_back("world_3_level_14_stats");
		world_3.push_back("world_3_level_15_stats");
		world_3.push_back("world_3_level_16_stats");
		world_3.push_back("world_3_level_17_stats");
		world_3.push_back("world_3_level_18_stats");
		world_3.push_back("world_3_level_19_stats");
		world_3.push_back("world_3_level_20_stats");
		world_3.push_back("world_3_level_21_stats");

		WORLDS_LEVELS_STATS.push_back(world_3);

		world_4.push_back("world_4_level_1_stats");
		world_4.push_back("world_4_level_2_stats");
		world_4.push_back("world_4_level_3_stats");
		world_4.push_back("world_4_level_4_stats");
		world_4.push_back("world_4_level_5_stats");
		world_4.push_back("world_4_level_6_stats");
		world_4.push_back("world_4_level_7_stats");
		world_4.push_back("world_4_level_8_stats");
		world_4.push_back("world_4_level_9_stats");
		world_4.push_back("world_4_level_10_stats");
		world_4.push_back("world_4_level_11_stats");
		world_4.push_back("world_4_level_12_stats");
		world_4.push_back("world_4_level_13_stats");
		world_4.push_back("world_4_level_14_stats");
		world_4.push_back("world_4_level_15_stats");
		world_4.push_back("world_4_level_16_stats");
		world_4.push_back("world_4_level_17_stats");
		world_4.push_back("world_4_level_18_stats");
		world_4.push_back("world_4_level_19_stats");
		world_4.push_back("world_4_level_20_stats");
		world_4.push_back("world_4_level_21_stats");

		WORLDS_LEVELS_STATS.push_back(world_4);

		world_5.push_back("world_5_level_1_stats");
		world_5.push_back("world_5_level_2_stats");
		world_5.push_back("world_5_level_3_stats");
		world_5.push_back("world_5_level_4_stats");
		world_5.push_back("world_5_level_5_stats");
		world_5.push_back("world_5_level_6_stats");
		world_5.push_back("world_5_level_7_stats");
		world_5.push_back("world_5_level_8_stats");
		world_5.push_back("world_5_level_9_stats");
		world_5.push_back("world_5_level_10_stats");
		world_5.push_back("world_5_level_11_stats");
		world_5.push_back("world_5_level_12_stats");
		world_5.push_back("world_5_level_13_stats");
		world_5.push_back("world_5_level_14_stats");
		world_5.push_back("world_5_level_15_stats");
		world_5.push_back("world_5_level_16_stats");
		world_5.push_back("world_5_level_17_stats");
		world_5.push_back("world_5_level_18_stats");
		world_5.push_back("world_5_level_19_stats");
		world_5.push_back("world_5_level_20_stats");
		world_5.push_back("world_5_level_21_stats");

		WORLDS_LEVELS_STATS.push_back(world_5);
	}

	int worldIndex = world - 1;
	int levelIndex = level - 1;

	return WORLDS_LEVELS_STATS[worldIndex][levelIndex];
}