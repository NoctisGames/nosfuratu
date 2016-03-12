#pragma once

namespace NosFURatu
{
	class SaveData
	{
	public:
		static bool isLevelComplete(int world, int level);
		static void setLevelComplete(int world, int level);

	private:
		static Platform::String^ getKeyForLevel(int world, int level);

		SaveData();
		~SaveData();
	};
}