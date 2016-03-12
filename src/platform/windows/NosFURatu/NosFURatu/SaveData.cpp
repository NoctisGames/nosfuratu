#include "SaveData.h"

#include <vector>

using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace NosFURatu;

bool SaveData::isLevelComplete(int world, int level)
{
	ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;

	ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);

	auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;

	Platform::String^ key = getKeyForLevel(world, level);

	bool hasValue = values->HasKey(key);
	
	if (hasValue)
	{
		bool value = safe_cast<bool>(values->Lookup(key));

		return value;
	}

	return false;
}

void SaveData::setLevelComplete(int world, int level)
{
	ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;

	ApplicationDataContainer^ container = localSettings->CreateContainer("nosFURatuContainer", ApplicationDataCreateDisposition::Always);

	auto values = localSettings->Containers->Lookup("nosFURatuContainer")->Values;

	Platform::String^ key = getKeyForLevel(world, level);

	values->Insert(key, PropertyValue::CreateBoolean(true));
}

Platform::String^ SaveData::getKeyForLevel(int world, int level)
{
	static std::vector<std::vector<Platform::String^>> WORLDS_LEVELS_COMPLETION;
	static std::vector<Platform::String^> world_1;
	static std::vector<Platform::String^> world_2;
	static std::vector<Platform::String^> world_3;
	static std::vector<Platform::String^> world_4;
	static std::vector<Platform::String^> world_5;

	if (WORLDS_LEVELS_COMPLETION.size() == 0)
	{
		world_1.push_back("is_world_1_level_1_complete");
		world_1.push_back("is_world_1_level_2_complete");
		world_1.push_back("is_world_1_level_3_complete");
		world_1.push_back("is_world_1_level_4_complete");
		world_1.push_back("is_world_1_level_5_complete");
		world_1.push_back("is_world_1_level_6_complete");
		world_1.push_back("is_world_1_level_7_complete");
		world_1.push_back("is_world_1_level_8_complete");
		world_1.push_back("is_world_1_level_9_complete");
		world_1.push_back("is_world_1_level_10_complete");
		world_1.push_back("is_world_1_level_11_complete");
		world_1.push_back("is_world_1_level_12_complete");
		world_1.push_back("is_world_1_level_13_complete");
		world_1.push_back("is_world_1_level_14_complete");
		world_1.push_back("is_world_1_level_15_complete");
		world_1.push_back("is_world_1_level_16_complete");
		world_1.push_back("is_world_1_level_17_complete");
		world_1.push_back("is_world_1_level_18_complete");
		world_1.push_back("is_world_1_level_19_complete");
		world_1.push_back("is_world_1_level_20_complete");
		world_1.push_back("is_world_1_level_21_complete");

		WORLDS_LEVELS_COMPLETION.push_back(world_1);

		world_2.push_back("is_world_2_level_1_complete");
		world_2.push_back("is_world_2_level_2_complete");
		world_2.push_back("is_world_2_level_3_complete");
		world_2.push_back("is_world_2_level_4_complete");
		world_2.push_back("is_world_2_level_5_complete");
		world_2.push_back("is_world_2_level_6_complete");
		world_2.push_back("is_world_2_level_7_complete");
		world_2.push_back("is_world_2_level_8_complete");
		world_2.push_back("is_world_2_level_9_complete");
		world_2.push_back("is_world_2_level_10_complete");
		world_2.push_back("is_world_2_level_11_complete");
		world_2.push_back("is_world_2_level_12_complete");
		world_2.push_back("is_world_2_level_13_complete");
		world_2.push_back("is_world_2_level_14_complete");
		world_2.push_back("is_world_2_level_15_complete");
		world_2.push_back("is_world_2_level_16_complete");
		world_2.push_back("is_world_2_level_17_complete");
		world_2.push_back("is_world_2_level_18_complete");
		world_2.push_back("is_world_2_level_19_complete");
		world_2.push_back("is_world_2_level_20_complete");
		world_2.push_back("is_world_2_level_21_complete");

		WORLDS_LEVELS_COMPLETION.push_back(world_2);

		world_3.push_back("is_world_3_level_1_complete");
		world_3.push_back("is_world_3_level_2_complete");
		world_3.push_back("is_world_3_level_3_complete");
		world_3.push_back("is_world_3_level_4_complete");
		world_3.push_back("is_world_3_level_5_complete");
		world_3.push_back("is_world_3_level_6_complete");
		world_3.push_back("is_world_3_level_7_complete");
		world_3.push_back("is_world_3_level_8_complete");
		world_3.push_back("is_world_3_level_9_complete");
		world_3.push_back("is_world_3_level_10_complete");
		world_3.push_back("is_world_3_level_11_complete");
		world_3.push_back("is_world_3_level_12_complete");
		world_3.push_back("is_world_3_level_13_complete");
		world_3.push_back("is_world_3_level_14_complete");
		world_3.push_back("is_world_3_level_15_complete");
		world_3.push_back("is_world_3_level_16_complete");
		world_3.push_back("is_world_3_level_17_complete");
		world_3.push_back("is_world_3_level_18_complete");
		world_3.push_back("is_world_3_level_19_complete");
		world_3.push_back("is_world_3_level_20_complete");
		world_3.push_back("is_world_3_level_21_complete");

		WORLDS_LEVELS_COMPLETION.push_back(world_3);

		world_4.push_back("is_world_4_level_1_complete");
		world_4.push_back("is_world_4_level_2_complete");
		world_4.push_back("is_world_4_level_3_complete");
		world_4.push_back("is_world_4_level_4_complete");
		world_4.push_back("is_world_4_level_5_complete");
		world_4.push_back("is_world_4_level_6_complete");
		world_4.push_back("is_world_4_level_7_complete");
		world_4.push_back("is_world_4_level_8_complete");
		world_4.push_back("is_world_4_level_9_complete");
		world_4.push_back("is_world_4_level_10_complete");
		world_4.push_back("is_world_4_level_11_complete");
		world_4.push_back("is_world_4_level_12_complete");
		world_4.push_back("is_world_4_level_13_complete");
		world_4.push_back("is_world_4_level_14_complete");
		world_4.push_back("is_world_4_level_15_complete");
		world_4.push_back("is_world_4_level_16_complete");
		world_4.push_back("is_world_4_level_17_complete");
		world_4.push_back("is_world_4_level_18_complete");
		world_4.push_back("is_world_4_level_19_complete");
		world_4.push_back("is_world_4_level_20_complete");
		world_4.push_back("is_world_4_level_21_complete");

		WORLDS_LEVELS_COMPLETION.push_back(world_4);

		world_5.push_back("is_world_5_level_1_complete");
		world_5.push_back("is_world_5_level_2_complete");
		world_5.push_back("is_world_5_level_3_complete");
		world_5.push_back("is_world_5_level_4_complete");
		world_5.push_back("is_world_5_level_5_complete");
		world_5.push_back("is_world_5_level_6_complete");
		world_5.push_back("is_world_5_level_7_complete");
		world_5.push_back("is_world_5_level_8_complete");
		world_5.push_back("is_world_5_level_9_complete");
		world_5.push_back("is_world_5_level_10_complete");
		world_5.push_back("is_world_5_level_11_complete");
		world_5.push_back("is_world_5_level_12_complete");
		world_5.push_back("is_world_5_level_13_complete");
		world_5.push_back("is_world_5_level_14_complete");
		world_5.push_back("is_world_5_level_15_complete");
		world_5.push_back("is_world_5_level_16_complete");
		world_5.push_back("is_world_5_level_17_complete");
		world_5.push_back("is_world_5_level_18_complete");
		world_5.push_back("is_world_5_level_19_complete");
		world_5.push_back("is_world_5_level_20_complete");
		world_5.push_back("is_world_5_level_21_complete");

		WORLDS_LEVELS_COMPLETION.push_back(world_5);
	}

	int worldIndex = world - 1;
	int levelIndex = level - 1;

	return WORLDS_LEVELS_COMPLETION[worldIndex][levelIndex];
}