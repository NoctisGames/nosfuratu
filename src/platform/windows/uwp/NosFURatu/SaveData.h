#pragma once

class SaveData
{
public:
	static int getNumGoldenCarrots();
	static void setNumGoldenCarrots(int numGoldenCarrots);

	static int getViewedCutscenesFlag();
	static void setViewedCutscenesFlag(int viewedCutscenesFlag);

	static int getJonUnlockedAbilitiesFlag();

	static int getLevelScore(int world, int level);

	static int getLevelStatsFlag(int world, int level);
	static void setLevelStatsFlag(int world, int level, int levelStatsFlag);

	static void setLevelComplete(int world, int level, int score, int levelStatsFlag, int jonUnlockedAbilitiesFlag);

	static int getScorePushedOnline(int world, int level);
	static void setScorePushedOnline(int world, int level, int score);

private:
	static Platform::String^ getKeyForNumGoldenCarrots();
	static Platform::String^ getKeyForJonUnlockedAbilitiesFlag();
	static Platform::String^ getKeyForViewedCutscenesFlag();

	static Platform::String^ getKeyForLevelScore(int world, int level);
	static Platform::String^ getKeyForLevelStats(int world, int level);
	static Platform::String^ getKeyForLevelOnlineScore(int world, int level);

	SaveData() {}
	~SaveData() {}
};
