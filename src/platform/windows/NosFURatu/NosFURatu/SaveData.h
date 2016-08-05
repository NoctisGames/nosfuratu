#pragma once

namespace NosFURatu
{
	class SaveData
	{
	public:
		static int getNumGoldenCarrots();
        static void setNumGoldenCarrots(int numGoldenCarrots);
        
        static int getJonUnlockedAbilitiesFlag();
        static void setJonUnlockedAbilitiesFlag(int jonUnlockedAbilitiesFlag);
        
        static int getJonViewedCutscenesFlag();
        static void setJonViewedCutscenesFlag(int jonViewedCutscenesFlag);
        
        static int getLevelScore(int world, int level);

        static int getLevelStatsFlag(int world, int level);
        
        static void setLevelComplete(int world, int level, int score, int levelStatsFlag);
        
        static int getScorePushedOnline(int world, int level);
        static void setScorePushedOnline(int world, int level, int score);

	private:
        static Platform::String^ getKeyForNumGoldenCarrots();
        static Platform::String^ getKeyForJonUnlockedAbilitiesFlag();
        static Platform::String^ getKeyForJonViewedCutscenesFlag();
        
		static Platform::String^ getKeyForLevelScore(int world, int level);
        static Platform::String^ getKeyForLevelStats(int world, int level);
        static Platform::String^ getKeyForLevelOnlineScore(int world, int level);

		SaveData();
		~SaveData();
	};
}