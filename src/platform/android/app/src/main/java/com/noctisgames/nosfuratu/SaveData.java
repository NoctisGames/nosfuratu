package com.noctisgames.nosfuratu;

public final class SaveData
{
    private static final String[][] WORLDS_LEVELS_STATS;

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
    }

    public static int getLevelStats(int world, int level)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        return AppPrefs.getInstance().getInt(WORLDS_LEVELS_STATS[worldIndex][levelIndex], 0);
    }

    public static void setLevelComplete(int world, int level, int goldenCarrotsFlag)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        AppPrefs.getInstance().set(WORLDS_LEVELS_STATS[worldIndex][levelIndex], goldenCarrotsFlag);
    }
}