#pragma once

namespace NosFURatu
{
	class SaveData
	{
	public:
		static int getLevelStats(int world, int level);
		static void setLevelComplete(int world, int level, int goldenCarrotsFlag);

	private:
		static Platform::String^ getKeyForLevel(int world, int level);

		SaveData();
		~SaveData();
	};
}