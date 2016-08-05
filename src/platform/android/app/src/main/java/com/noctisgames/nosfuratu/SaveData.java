package com.noctisgames.nosfuratu;

public final class SaveData
{
    private static final String NUM_GOLDEN_CARROTS = "num_golden_carrots";
    private static final String JON_UNLOCKED_ABILITIES_FLAG = "jon_unlocked_abilities_flag";
    private static final String JON_VIEWED_CUTSCENES_FLAG = "jon_viewed_cutscenes_flag";
    private static final String[][] WORLDS_LEVELS_STATS;
    private static final String[][] WORLDS_LEVELS_SCORES;
    private static final String[][] WORLDS_LEVELS_SCORES_ONLINE;

    static
    {
        WORLDS_LEVELS_STATS = new String[][]
                {
                        {
                                "world_1_level_1_stats",
                                "world_1_level_2_stats",
                                "world_1_level_3_stats",
                                "world_1_level_4_stats",
                                "world_1_level_5_stats",
                                "world_1_level_6_stats",
                                "world_1_level_7_stats",
                                "world_1_level_8_stats",
                                "world_1_level_9_stats",
                                "world_1_level_10_stats",
                                "world_1_level_11_stats",
                                "world_1_level_12_stats",
                                "world_1_level_13_stats",
                                "world_1_level_14_stats",
                                "world_1_level_15_stats",
                                "world_1_level_16_stats",
                                "world_1_level_17_stats",
                                "world_1_level_18_stats",
                                "world_1_level_19_stats",
                                "world_1_level_20_stats",
                                "world_1_level_21_stats"
                        },

                        {
                                "world_2_level_1_stats",
                                "world_2_level_2_stats",
                                "world_2_level_3_stats",
                                "world_2_level_4_stats",
                                "world_2_level_5_stats",
                                "world_2_level_6_stats",
                                "world_2_level_7_stats",
                                "world_2_level_8_stats",
                                "world_2_level_9_stats",
                                "world_2_level_10_stats",
                                "world_2_level_11_stats",
                                "world_2_level_12_stats",
                                "world_2_level_13_stats",
                                "world_2_level_14_stats",
                                "world_2_level_15_stats",
                                "world_2_level_16_stats",
                                "world_2_level_17_stats",
                                "world_2_level_18_stats",
                                "world_2_level_19_stats",
                                "world_2_level_20_stats",
                                "world_2_level_21_stats"
                        },

                        {
                                "world_3_level_1_stats",
                                "world_3_level_2_stats",
                                "world_3_level_3_stats",
                                "world_3_level_4_stats",
                                "world_3_level_5_stats",
                                "world_3_level_6_stats",
                                "world_3_level_7_stats",
                                "world_3_level_8_stats",
                                "world_3_level_9_stats",
                                "world_3_level_10_stats",
                                "world_3_level_11_stats",
                                "world_3_level_12_stats",
                                "world_3_level_13_stats",
                                "world_3_level_14_stats",
                                "world_3_level_15_stats",
                                "world_3_level_16_stats",
                                "world_3_level_17_stats",
                                "world_3_level_18_stats",
                                "world_3_level_19_stats",
                                "world_3_level_20_stats",
                                "world_3_level_21_stats"
                        },

                        {
                                "world_4_level_1_stats",
                                "world_4_level_2_stats",
                                "world_4_level_3_stats",
                                "world_4_level_4_stats",
                                "world_4_level_5_stats",
                                "world_4_level_6_stats",
                                "world_4_level_7_stats",
                                "world_4_level_8_stats",
                                "world_4_level_9_stats",
                                "world_4_level_10_stats",
                                "world_4_level_11_stats",
                                "world_4_level_12_stats",
                                "world_4_level_13_stats",
                                "world_4_level_14_stats",
                                "world_4_level_15_stats",
                                "world_4_level_16_stats",
                                "world_4_level_17_stats",
                                "world_4_level_18_stats",
                                "world_4_level_19_stats",
                                "world_4_level_20_stats",
                                "world_4_level_21_stats"
                        },

                        {
                                "world_5_level_1_stats",
                                "world_5_level_2_stats",
                                "world_5_level_3_stats",
                                "world_5_level_4_stats",
                                "world_5_level_5_stats",
                                "world_5_level_6_stats",
                                "world_5_level_7_stats",
                                "world_5_level_8_stats",
                                "world_5_level_9_stats",
                                "world_5_level_10_stats",
                                "world_5_level_11_stats",
                                "world_5_level_12_stats",
                                "world_5_level_13_stats",
                                "world_5_level_14_stats",
                                "world_5_level_15_stats",
                                "world_5_level_16_stats",
                                "world_5_level_17_stats",
                                "world_5_level_18_stats",
                                "world_5_level_19_stats",
                                "world_5_level_20_stats",
                                "world_5_level_21_stats"
                        }
                };

        WORLDS_LEVELS_SCORES = new String[][]
                {
                        {
                                "world_1_level_1_scores",
                                "world_1_level_2_scores",
                                "world_1_level_3_scores",
                                "world_1_level_4_scores",
                                "world_1_level_5_scores",
                                "world_1_level_6_scores",
                                "world_1_level_7_scores",
                                "world_1_level_8_scores",
                                "world_1_level_9_scores",
                                "world_1_level_10_scores",
                                "world_1_level_11_scores",
                                "world_1_level_12_scores",
                                "world_1_level_13_scores",
                                "world_1_level_14_scores",
                                "world_1_level_15_scores",
                                "world_1_level_16_scores",
                                "world_1_level_17_scores",
                                "world_1_level_18_scores",
                                "world_1_level_19_scores",
                                "world_1_level_20_scores",
                                "world_1_level_21_scores"
                        },

                        {
                                "world_2_level_1_scores",
                                "world_2_level_2_scores",
                                "world_2_level_3_scores",
                                "world_2_level_4_scores",
                                "world_2_level_5_scores",
                                "world_2_level_6_scores",
                                "world_2_level_7_scores",
                                "world_2_level_8_scores",
                                "world_2_level_9_scores",
                                "world_2_level_10_scores",
                                "world_2_level_11_scores",
                                "world_2_level_12_scores",
                                "world_2_level_13_scores",
                                "world_2_level_14_scores",
                                "world_2_level_15_scores",
                                "world_2_level_16_scores",
                                "world_2_level_17_scores",
                                "world_2_level_18_scores",
                                "world_2_level_19_scores",
                                "world_2_level_20_scores",
                                "world_2_level_21_scores"
                        },

                        {
                                "world_3_level_1_scores",
                                "world_3_level_2_scores",
                                "world_3_level_3_scores",
                                "world_3_level_4_scores",
                                "world_3_level_5_scores",
                                "world_3_level_6_scores",
                                "world_3_level_7_scores",
                                "world_3_level_8_scores",
                                "world_3_level_9_scores",
                                "world_3_level_10_scores",
                                "world_3_level_11_scores",
                                "world_3_level_12_scores",
                                "world_3_level_13_scores",
                                "world_3_level_14_scores",
                                "world_3_level_15_scores",
                                "world_3_level_16_scores",
                                "world_3_level_17_scores",
                                "world_3_level_18_scores",
                                "world_3_level_19_scores",
                                "world_3_level_20_scores",
                                "world_3_level_21_scores"
                        },

                        {
                                "world_4_level_1_scores",
                                "world_4_level_2_scores",
                                "world_4_level_3_scores",
                                "world_4_level_4_scores",
                                "world_4_level_5_scores",
                                "world_4_level_6_scores",
                                "world_4_level_7_scores",
                                "world_4_level_8_scores",
                                "world_4_level_9_scores",
                                "world_4_level_10_scores",
                                "world_4_level_11_scores",
                                "world_4_level_12_scores",
                                "world_4_level_13_scores",
                                "world_4_level_14_scores",
                                "world_4_level_15_scores",
                                "world_4_level_16_scores",
                                "world_4_level_17_scores",
                                "world_4_level_18_scores",
                                "world_4_level_19_scores",
                                "world_4_level_20_scores",
                                "world_4_level_21_scores"
                        },

                        {
                                "world_5_level_1_scores",
                                "world_5_level_2_scores",
                                "world_5_level_3_scores",
                                "world_5_level_4_scores",
                                "world_5_level_5_scores",
                                "world_5_level_6_scores",
                                "world_5_level_7_scores",
                                "world_5_level_8_scores",
                                "world_5_level_9_scores",
                                "world_5_level_10_scores",
                                "world_5_level_11_scores",
                                "world_5_level_12_scores",
                                "world_5_level_13_scores",
                                "world_5_level_14_scores",
                                "world_5_level_15_scores",
                                "world_5_level_16_scores",
                                "world_5_level_17_scores",
                                "world_5_level_18_scores",
                                "world_5_level_19_scores",
                                "world_5_level_20_scores",
                                "world_5_level_21_scores"
                        }
                };

        WORLDS_LEVELS_SCORES_ONLINE = new String[][]
                {
                        {
                                "world_1_level_1_scores_online",
                                "world_1_level_2_scores_online",
                                "world_1_level_3_scores_online",
                                "world_1_level_4_scores_online",
                                "world_1_level_5_scores_online",
                                "world_1_level_6_scores_online",
                                "world_1_level_7_scores_online",
                                "world_1_level_8_scores_online",
                                "world_1_level_9_scores_online",
                                "world_1_level_10_scores_online",
                                "world_1_level_11_scores_online",
                                "world_1_level_12_scores_online",
                                "world_1_level_13_scores_online",
                                "world_1_level_14_scores_online",
                                "world_1_level_15_scores_online",
                                "world_1_level_16_scores_online",
                                "world_1_level_17_scores_online",
                                "world_1_level_18_scores_online",
                                "world_1_level_19_scores_online",
                                "world_1_level_20_scores_online",
                                "world_1_level_21_scores_online"
                        },

                        {
                                "world_2_level_1_scores_online",
                                "world_2_level_2_scores_online",
                                "world_2_level_3_scores_online",
                                "world_2_level_4_scores_online",
                                "world_2_level_5_scores_online",
                                "world_2_level_6_scores_online",
                                "world_2_level_7_scores_online",
                                "world_2_level_8_scores_online",
                                "world_2_level_9_scores_online",
                                "world_2_level_10_scores_online",
                                "world_2_level_11_scores_online",
                                "world_2_level_12_scores_online",
                                "world_2_level_13_scores_online",
                                "world_2_level_14_scores_online",
                                "world_2_level_15_scores_online",
                                "world_2_level_16_scores_online",
                                "world_2_level_17_scores_online",
                                "world_2_level_18_scores_online",
                                "world_2_level_19_scores_online",
                                "world_2_level_20_scores_online",
                                "world_2_level_21_scores_online"
                        },

                        {
                                "world_3_level_1_scores_online",
                                "world_3_level_2_scores_online",
                                "world_3_level_3_scores_online",
                                "world_3_level_4_scores_online",
                                "world_3_level_5_scores_online",
                                "world_3_level_6_scores_online",
                                "world_3_level_7_scores_online",
                                "world_3_level_8_scores_online",
                                "world_3_level_9_scores_online",
                                "world_3_level_10_scores_online",
                                "world_3_level_11_scores_online",
                                "world_3_level_12_scores_online",
                                "world_3_level_13_scores_online",
                                "world_3_level_14_scores_online",
                                "world_3_level_15_scores_online",
                                "world_3_level_16_scores_online",
                                "world_3_level_17_scores_online",
                                "world_3_level_18_scores_online",
                                "world_3_level_19_scores_online",
                                "world_3_level_20_scores_online",
                                "world_3_level_21_scores_online"
                        },

                        {
                                "world_4_level_1_scores_online",
                                "world_4_level_2_scores_online",
                                "world_4_level_3_scores_online",
                                "world_4_level_4_scores_online",
                                "world_4_level_5_scores_online",
                                "world_4_level_6_scores_online",
                                "world_4_level_7_scores_online",
                                "world_4_level_8_scores_online",
                                "world_4_level_9_scores_online",
                                "world_4_level_10_scores_online",
                                "world_4_level_11_scores_online",
                                "world_4_level_12_scores_online",
                                "world_4_level_13_scores_online",
                                "world_4_level_14_scores_online",
                                "world_4_level_15_scores_online",
                                "world_4_level_16_scores_online",
                                "world_4_level_17_scores_online",
                                "world_4_level_18_scores_online",
                                "world_4_level_19_scores_online",
                                "world_4_level_20_scores_online",
                                "world_4_level_21_scores_online"
                        },

                        {
                                "world_5_level_1_scores_online",
                                "world_5_level_2_scores_online",
                                "world_5_level_3_scores_online",
                                "world_5_level_4_scores_online",
                                "world_5_level_5_scores_online",
                                "world_5_level_6_scores_online",
                                "world_5_level_7_scores_online",
                                "world_5_level_8_scores_online",
                                "world_5_level_9_scores_online",
                                "world_5_level_10_scores_online",
                                "world_5_level_11_scores_online",
                                "world_5_level_12_scores_online",
                                "world_5_level_13_scores_online",
                                "world_5_level_14_scores_online",
                                "world_5_level_15_scores_online",
                                "world_5_level_16_scores_online",
                                "world_5_level_17_scores_online",
                                "world_5_level_18_scores_online",
                                "world_5_level_19_scores_online",
                                "world_5_level_20_scores_online",
                                "world_5_level_21_scores_online"
                        }
                };
    }

    public static int getNumGoldenCarrots()
    {
        return AppPrefs.getInstance().getInt(NUM_GOLDEN_CARROTS, 0);
    }

    public static void setNumGoldenCarrots(int numGoldenCarrots)
    {
        AppPrefs.getInstance().set(NUM_GOLDEN_CARROTS, numGoldenCarrots);
    }

    public static int getJonUnlockedAbilitiesFlag()
    {
        return AppPrefs.getInstance().getInt(JON_UNLOCKED_ABILITIES_FLAG, 0);
    }

    public static void setJonUnlockedAbilitiesFlag(int jonUnlockedAbilitiesFlag)
    {
        AppPrefs.getInstance().set(JON_UNLOCKED_ABILITIES_FLAG, jonUnlockedAbilitiesFlag);
    }

    public static int getJonViewedCutscenesFlag()
    {
        return AppPrefs.getInstance().getInt(JON_VIEWED_CUTSCENES_FLAG, 0);
    }

    public static void setJonViewedCutscenesFlag(int jonViewedCutscenesFlag)
    {
        AppPrefs.getInstance().set(JON_VIEWED_CUTSCENES_FLAG, jonViewedCutscenesFlag);
    }

    public static int getLevelScore(int world, int level)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        return AppPrefs.getInstance().getInt(WORLDS_LEVELS_SCORES[worldIndex][levelIndex], 0);
    }

    public static int getLevelStatsFlag(int world, int level)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        return AppPrefs.getInstance().getInt(WORLDS_LEVELS_STATS[worldIndex][levelIndex], 0);
    }

    public static void setLevelComplete(int world, int level, int score, int levelStatsFlag)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        AppPrefs.getInstance().set(WORLDS_LEVELS_SCORES[worldIndex][levelIndex], score);
        AppPrefs.getInstance().set(WORLDS_LEVELS_STATS[worldIndex][levelIndex], levelStatsFlag);
    }

    public static int getScorePushedOnline(int world, int level)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        return AppPrefs.getInstance().getInt(WORLDS_LEVELS_SCORES_ONLINE[worldIndex][levelIndex], 0);
    }

    public static void setScorePushedOnline(int world, int level, int score)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        AppPrefs.getInstance().set(WORLDS_LEVELS_SCORES_ONLINE[worldIndex][levelIndex], score);
    }
}