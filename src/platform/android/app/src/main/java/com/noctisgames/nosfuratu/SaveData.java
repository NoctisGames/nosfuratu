package com.noctisgames.nosfuratu;

public final class SaveData
{
    private static final String[][] WORLDS_LEVELS_COMPLETION;

    static
    {
        WORLDS_LEVELS_COMPLETION = new String[][]
                {
                        {
                                "is_world_1_level_1_complete",
                                "is_world_1_level_2_complete",
                                "is_world_1_level_3_complete",
                                "is_world_1_level_4_complete",
                                "is_world_1_level_5_complete",
                                "is_world_1_level_6_complete",
                                "is_world_1_level_7_complete",
                                "is_world_1_level_8_complete",
                                "is_world_1_level_9_complete",
                                "is_world_1_level_10_complete",
                                "is_world_1_level_11_complete",
                                "is_world_1_level_12_complete",
                                "is_world_1_level_13_complete",
                                "is_world_1_level_14_complete",
                                "is_world_1_level_15_complete",
                                "is_world_1_level_16_complete",
                                "is_world_1_level_17_complete",
                                "is_world_1_level_18_complete",
                                "is_world_1_level_19_complete",
                                "is_world_1_level_20_complete",
                                "is_world_1_level_21_complete"
                        },

                        {
                                "is_world_2_level_1_complete",
                                "is_world_2_level_2_complete",
                                "is_world_2_level_3_complete",
                                "is_world_2_level_4_complete",
                                "is_world_2_level_5_complete",
                                "is_world_2_level_6_complete",
                                "is_world_2_level_7_complete",
                                "is_world_2_level_8_complete",
                                "is_world_2_level_9_complete",
                                "is_world_2_level_10_complete",
                                "is_world_2_level_11_complete",
                                "is_world_2_level_12_complete",
                                "is_world_2_level_13_complete",
                                "is_world_2_level_14_complete",
                                "is_world_2_level_15_complete",
                                "is_world_2_level_16_complete",
                                "is_world_2_level_17_complete",
                                "is_world_2_level_18_complete",
                                "is_world_2_level_19_complete",
                                "is_world_2_level_20_complete",
                                "is_world_2_level_21_complete"
                        },

                        {
                                "is_world_3_level_1_complete",
                                "is_world_3_level_2_complete",
                                "is_world_3_level_3_complete",
                                "is_world_3_level_4_complete",
                                "is_world_3_level_5_complete",
                                "is_world_3_level_6_complete",
                                "is_world_3_level_7_complete",
                                "is_world_3_level_8_complete",
                                "is_world_3_level_9_complete",
                                "is_world_3_level_10_complete",
                                "is_world_3_level_11_complete",
                                "is_world_3_level_12_complete",
                                "is_world_3_level_13_complete",
                                "is_world_3_level_14_complete",
                                "is_world_3_level_15_complete",
                                "is_world_3_level_16_complete",
                                "is_world_3_level_17_complete",
                                "is_world_3_level_18_complete",
                                "is_world_3_level_19_complete",
                                "is_world_3_level_20_complete",
                                "is_world_3_level_21_complete"
                        },

                        {
                                "is_world_4_level_1_complete",
                                "is_world_4_level_2_complete",
                                "is_world_4_level_3_complete",
                                "is_world_4_level_4_complete",
                                "is_world_4_level_5_complete",
                                "is_world_4_level_6_complete",
                                "is_world_4_level_7_complete",
                                "is_world_4_level_8_complete",
                                "is_world_4_level_9_complete",
                                "is_world_4_level_10_complete",
                                "is_world_4_level_11_complete",
                                "is_world_4_level_12_complete",
                                "is_world_4_level_13_complete",
                                "is_world_4_level_14_complete",
                                "is_world_4_level_15_complete",
                                "is_world_4_level_16_complete",
                                "is_world_4_level_17_complete",
                                "is_world_4_level_18_complete",
                                "is_world_4_level_19_complete",
                                "is_world_4_level_20_complete",
                                "is_world_4_level_21_complete"
                        },

                        {
                                "is_world_5_level_1_complete",
                                "is_world_5_level_2_complete",
                                "is_world_5_level_3_complete",
                                "is_world_5_level_4_complete",
                                "is_world_5_level_5_complete",
                                "is_world_5_level_6_complete",
                                "is_world_5_level_7_complete",
                                "is_world_5_level_8_complete",
                                "is_world_5_level_9_complete",
                                "is_world_5_level_10_complete",
                                "is_world_5_level_11_complete",
                                "is_world_5_level_12_complete",
                                "is_world_5_level_13_complete",
                                "is_world_5_level_14_complete",
                                "is_world_5_level_15_complete",
                                "is_world_5_level_16_complete",
                                "is_world_5_level_17_complete",
                                "is_world_5_level_18_complete",
                                "is_world_5_level_19_complete",
                                "is_world_5_level_20_complete",
                                "is_world_5_level_21_complete"
                        }
                };
    }

    public static boolean isLevelComplete(int world, int level)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        return AppPrefs.getInstance().getBoolean(WORLDS_LEVELS_COMPLETION[worldIndex][levelIndex], false);
    }

    public static void setLevelComplete(int world, int level)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        AppPrefs.getInstance().set(WORLDS_LEVELS_COMPLETION[worldIndex][levelIndex], true);
    }
}