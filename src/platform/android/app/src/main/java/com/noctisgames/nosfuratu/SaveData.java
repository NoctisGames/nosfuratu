package com.noctisgames.nosfuratu;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;

public final class SaveData
{
    private static final String NUM_GOLDEN_CARROTS = "v1_0_0_num_golden_carrots";
    private static final String JON_UNLOCKED_ABILITIES_FLAG = "v1_0_0_jon_unlocked_abilities_flag";
    private static final String VIEWED_CUTSCENES_FLAG = "v1_0_0_viewed_cutscenes_flag";
    private static final String[][] WORLDS_LEVELS_STATS;
    private static final String[][] WORLDS_LEVELS_SCORES;
    private static final String[][] WORLDS_LEVELS_SCORES_ONLINE;

    private static SharedPreferences _securePrefs;

    public static void init(Activity activity)
    {
        _securePrefs = new ObscuredSharedPreferences(activity, activity.getSharedPreferences("Noctis_Games_NosFURatu", Context.MODE_PRIVATE));
    }

    static
    {
        WORLDS_LEVELS_STATS = new String[][]
                {
                        {
                                "v1_0_0_world_1_level_1_stats",
                                "v1_0_0_world_1_level_2_stats",
                                "v1_0_0_world_1_level_3_stats",
                                "v1_0_0_world_1_level_4_stats",
                                "v1_0_0_world_1_level_5_stats",
                                "v1_0_0_world_1_level_6_stats",
                                "v1_0_0_world_1_level_7_stats",
                                "v1_0_0_world_1_level_8_stats",
                                "v1_0_0_world_1_level_9_stats",
                                "v1_0_0_world_1_level_10_stats",
                                "v1_0_0_world_1_level_11_stats",
                                "v1_0_0_world_1_level_12_stats",
                                "v1_0_0_world_1_level_13_stats",
                                "v1_0_0_world_1_level_14_stats",
                                "v1_0_0_world_1_level_15_stats",
                                "v1_0_0_world_1_level_16_stats",
                                "v1_0_0_world_1_level_17_stats",
                                "v1_0_0_world_1_level_18_stats",
                                "v1_0_0_world_1_level_19_stats",
                                "v1_0_0_world_1_level_20_stats",
                                "v1_0_0_world_1_level_21_stats"
                        },

                        {
                                "v1_0_0_world_2_level_1_stats",
                                "v1_0_0_world_2_level_2_stats",
                                "v1_0_0_world_2_level_3_stats",
                                "v1_0_0_world_2_level_4_stats",
                                "v1_0_0_world_2_level_5_stats",
                                "v1_0_0_world_2_level_6_stats",
                                "v1_0_0_world_2_level_7_stats",
                                "v1_0_0_world_2_level_8_stats",
                                "v1_0_0_world_2_level_9_stats",
                                "v1_0_0_world_2_level_10_stats",
                                "v1_0_0_world_2_level_11_stats",
                                "v1_0_0_world_2_level_12_stats",
                                "v1_0_0_world_2_level_13_stats",
                                "v1_0_0_world_2_level_14_stats",
                                "v1_0_0_world_2_level_15_stats",
                                "v1_0_0_world_2_level_16_stats",
                                "v1_0_0_world_2_level_17_stats",
                                "v1_0_0_world_2_level_18_stats",
                                "v1_0_0_world_2_level_19_stats",
                                "v1_0_0_world_2_level_20_stats",
                                "v1_0_0_world_2_level_21_stats"
                        },

                        {
                                "v1_0_0_world_3_level_1_stats",
                                "v1_0_0_world_3_level_2_stats",
                                "v1_0_0_world_3_level_3_stats",
                                "v1_0_0_world_3_level_4_stats",
                                "v1_0_0_world_3_level_5_stats",
                                "v1_0_0_world_3_level_6_stats",
                                "v1_0_0_world_3_level_7_stats",
                                "v1_0_0_world_3_level_8_stats",
                                "v1_0_0_world_3_level_9_stats",
                                "v1_0_0_world_3_level_10_stats",
                                "v1_0_0_world_3_level_11_stats",
                                "v1_0_0_world_3_level_12_stats",
                                "v1_0_0_world_3_level_13_stats",
                                "v1_0_0_world_3_level_14_stats",
                                "v1_0_0_world_3_level_15_stats",
                                "v1_0_0_world_3_level_16_stats",
                                "v1_0_0_world_3_level_17_stats",
                                "v1_0_0_world_3_level_18_stats",
                                "v1_0_0_world_3_level_19_stats",
                                "v1_0_0_world_3_level_20_stats",
                                "v1_0_0_world_3_level_21_stats"
                        },

                        {
                                "v1_0_0_world_4_level_1_stats",
                                "v1_0_0_world_4_level_2_stats",
                                "v1_0_0_world_4_level_3_stats",
                                "v1_0_0_world_4_level_4_stats",
                                "v1_0_0_world_4_level_5_stats",
                                "v1_0_0_world_4_level_6_stats",
                                "v1_0_0_world_4_level_7_stats",
                                "v1_0_0_world_4_level_8_stats",
                                "v1_0_0_world_4_level_9_stats",
                                "v1_0_0_world_4_level_10_stats",
                                "v1_0_0_world_4_level_11_stats",
                                "v1_0_0_world_4_level_12_stats",
                                "v1_0_0_world_4_level_13_stats",
                                "v1_0_0_world_4_level_14_stats",
                                "v1_0_0_world_4_level_15_stats",
                                "v1_0_0_world_4_level_16_stats",
                                "v1_0_0_world_4_level_17_stats",
                                "v1_0_0_world_4_level_18_stats",
                                "v1_0_0_world_4_level_19_stats",
                                "v1_0_0_world_4_level_20_stats",
                                "v1_0_0_world_4_level_21_stats"
                        },

                        {
                                "v1_0_0_world_5_level_1_stats",
                                "v1_0_0_world_5_level_2_stats",
                                "v1_0_0_world_5_level_3_stats",
                                "v1_0_0_world_5_level_4_stats",
                                "v1_0_0_world_5_level_5_stats",
                                "v1_0_0_world_5_level_6_stats",
                                "v1_0_0_world_5_level_7_stats",
                                "v1_0_0_world_5_level_8_stats",
                                "v1_0_0_world_5_level_9_stats",
                                "v1_0_0_world_5_level_10_stats",
                                "v1_0_0_world_5_level_11_stats",
                                "v1_0_0_world_5_level_12_stats",
                                "v1_0_0_world_5_level_13_stats",
                                "v1_0_0_world_5_level_14_stats",
                                "v1_0_0_world_5_level_15_stats",
                                "v1_0_0_world_5_level_16_stats",
                                "v1_0_0_world_5_level_17_stats",
                                "v1_0_0_world_5_level_18_stats",
                                "v1_0_0_world_5_level_19_stats",
                                "v1_0_0_world_5_level_20_stats",
                                "v1_0_0_world_5_level_21_stats"
                        }
                };

        WORLDS_LEVELS_SCORES = new String[][]
                {
                        {
                                "v1_0_0_world_1_level_1_scores",
                                "v1_0_0_world_1_level_2_scores",
                                "v1_0_0_world_1_level_3_scores",
                                "v1_0_0_world_1_level_4_scores",
                                "v1_0_0_world_1_level_5_scores",
                                "v1_0_0_world_1_level_6_scores",
                                "v1_0_0_world_1_level_7_scores",
                                "v1_0_0_world_1_level_8_scores",
                                "v1_0_0_world_1_level_9_scores",
                                "v1_0_0_world_1_level_10_scores",
                                "v1_0_0_world_1_level_11_scores",
                                "v1_0_0_world_1_level_12_scores",
                                "v1_0_0_world_1_level_13_scores",
                                "v1_0_0_world_1_level_14_scores",
                                "v1_0_0_world_1_level_15_scores",
                                "v1_0_0_world_1_level_16_scores",
                                "v1_0_0_world_1_level_17_scores",
                                "v1_0_0_world_1_level_18_scores",
                                "v1_0_0_world_1_level_19_scores",
                                "v1_0_0_world_1_level_20_scores",
                                "v1_0_0_world_1_level_21_scores"
                        },

                        {
                                "v1_0_0_world_2_level_1_scores",
                                "v1_0_0_world_2_level_2_scores",
                                "v1_0_0_world_2_level_3_scores",
                                "v1_0_0_world_2_level_4_scores",
                                "v1_0_0_world_2_level_5_scores",
                                "v1_0_0_world_2_level_6_scores",
                                "v1_0_0_world_2_level_7_scores",
                                "v1_0_0_world_2_level_8_scores",
                                "v1_0_0_world_2_level_9_scores",
                                "v1_0_0_world_2_level_10_scores",
                                "v1_0_0_world_2_level_11_scores",
                                "v1_0_0_world_2_level_12_scores",
                                "v1_0_0_world_2_level_13_scores",
                                "v1_0_0_world_2_level_14_scores",
                                "v1_0_0_world_2_level_15_scores",
                                "v1_0_0_world_2_level_16_scores",
                                "v1_0_0_world_2_level_17_scores",
                                "v1_0_0_world_2_level_18_scores",
                                "v1_0_0_world_2_level_19_scores",
                                "v1_0_0_world_2_level_20_scores",
                                "v1_0_0_world_2_level_21_scores"
                        },

                        {
                                "v1_0_0_world_3_level_1_scores",
                                "v1_0_0_world_3_level_2_scores",
                                "v1_0_0_world_3_level_3_scores",
                                "v1_0_0_world_3_level_4_scores",
                                "v1_0_0_world_3_level_5_scores",
                                "v1_0_0_world_3_level_6_scores",
                                "v1_0_0_world_3_level_7_scores",
                                "v1_0_0_world_3_level_8_scores",
                                "v1_0_0_world_3_level_9_scores",
                                "v1_0_0_world_3_level_10_scores",
                                "v1_0_0_world_3_level_11_scores",
                                "v1_0_0_world_3_level_12_scores",
                                "v1_0_0_world_3_level_13_scores",
                                "v1_0_0_world_3_level_14_scores",
                                "v1_0_0_world_3_level_15_scores",
                                "v1_0_0_world_3_level_16_scores",
                                "v1_0_0_world_3_level_17_scores",
                                "v1_0_0_world_3_level_18_scores",
                                "v1_0_0_world_3_level_19_scores",
                                "v1_0_0_world_3_level_20_scores",
                                "v1_0_0_world_3_level_21_scores"
                        },

                        {
                                "v1_0_0_world_4_level_1_scores",
                                "v1_0_0_world_4_level_2_scores",
                                "v1_0_0_world_4_level_3_scores",
                                "v1_0_0_world_4_level_4_scores",
                                "v1_0_0_world_4_level_5_scores",
                                "v1_0_0_world_4_level_6_scores",
                                "v1_0_0_world_4_level_7_scores",
                                "v1_0_0_world_4_level_8_scores",
                                "v1_0_0_world_4_level_9_scores",
                                "v1_0_0_world_4_level_10_scores",
                                "v1_0_0_world_4_level_11_scores",
                                "v1_0_0_world_4_level_12_scores",
                                "v1_0_0_world_4_level_13_scores",
                                "v1_0_0_world_4_level_14_scores",
                                "v1_0_0_world_4_level_15_scores",
                                "v1_0_0_world_4_level_16_scores",
                                "v1_0_0_world_4_level_17_scores",
                                "v1_0_0_world_4_level_18_scores",
                                "v1_0_0_world_4_level_19_scores",
                                "v1_0_0_world_4_level_20_scores",
                                "v1_0_0_world_4_level_21_scores"
                        },

                        {
                                "v1_0_0_world_5_level_1_scores",
                                "v1_0_0_world_5_level_2_scores",
                                "v1_0_0_world_5_level_3_scores",
                                "v1_0_0_world_5_level_4_scores",
                                "v1_0_0_world_5_level_5_scores",
                                "v1_0_0_world_5_level_6_scores",
                                "v1_0_0_world_5_level_7_scores",
                                "v1_0_0_world_5_level_8_scores",
                                "v1_0_0_world_5_level_9_scores",
                                "v1_0_0_world_5_level_10_scores",
                                "v1_0_0_world_5_level_11_scores",
                                "v1_0_0_world_5_level_12_scores",
                                "v1_0_0_world_5_level_13_scores",
                                "v1_0_0_world_5_level_14_scores",
                                "v1_0_0_world_5_level_15_scores",
                                "v1_0_0_world_5_level_16_scores",
                                "v1_0_0_world_5_level_17_scores",
                                "v1_0_0_world_5_level_18_scores",
                                "v1_0_0_world_5_level_19_scores",
                                "v1_0_0_world_5_level_20_scores",
                                "v1_0_0_world_5_level_21_scores"
                        }
                };

        WORLDS_LEVELS_SCORES_ONLINE = new String[][]
                {
                        {
                                "v1_0_0_world_1_level_1_scores_online",
                                "v1_0_0_world_1_level_2_scores_online",
                                "v1_0_0_world_1_level_3_scores_online",
                                "v1_0_0_world_1_level_4_scores_online",
                                "v1_0_0_world_1_level_5_scores_online",
                                "v1_0_0_world_1_level_6_scores_online",
                                "v1_0_0_world_1_level_7_scores_online",
                                "v1_0_0_world_1_level_8_scores_online",
                                "v1_0_0_world_1_level_9_scores_online",
                                "v1_0_0_world_1_level_10_scores_online",
                                "v1_0_0_world_1_level_11_scores_online",
                                "v1_0_0_world_1_level_12_scores_online",
                                "v1_0_0_world_1_level_13_scores_online",
                                "v1_0_0_world_1_level_14_scores_online",
                                "v1_0_0_world_1_level_15_scores_online",
                                "v1_0_0_world_1_level_16_scores_online",
                                "v1_0_0_world_1_level_17_scores_online",
                                "v1_0_0_world_1_level_18_scores_online",
                                "v1_0_0_world_1_level_19_scores_online",
                                "v1_0_0_world_1_level_20_scores_online",
                                "v1_0_0_world_1_level_21_scores_online"
                        },

                        {
                                "v1_0_0_world_2_level_1_scores_online",
                                "v1_0_0_world_2_level_2_scores_online",
                                "v1_0_0_world_2_level_3_scores_online",
                                "v1_0_0_world_2_level_4_scores_online",
                                "v1_0_0_world_2_level_5_scores_online",
                                "v1_0_0_world_2_level_6_scores_online",
                                "v1_0_0_world_2_level_7_scores_online",
                                "v1_0_0_world_2_level_8_scores_online",
                                "v1_0_0_world_2_level_9_scores_online",
                                "v1_0_0_world_2_level_10_scores_online",
                                "v1_0_0_world_2_level_11_scores_online",
                                "v1_0_0_world_2_level_12_scores_online",
                                "v1_0_0_world_2_level_13_scores_online",
                                "v1_0_0_world_2_level_14_scores_online",
                                "v1_0_0_world_2_level_15_scores_online",
                                "v1_0_0_world_2_level_16_scores_online",
                                "v1_0_0_world_2_level_17_scores_online",
                                "v1_0_0_world_2_level_18_scores_online",
                                "v1_0_0_world_2_level_19_scores_online",
                                "v1_0_0_world_2_level_20_scores_online",
                                "v1_0_0_world_2_level_21_scores_online"
                        },

                        {
                                "v1_0_0_world_3_level_1_scores_online",
                                "v1_0_0_world_3_level_2_scores_online",
                                "v1_0_0_world_3_level_3_scores_online",
                                "v1_0_0_world_3_level_4_scores_online",
                                "v1_0_0_world_3_level_5_scores_online",
                                "v1_0_0_world_3_level_6_scores_online",
                                "v1_0_0_world_3_level_7_scores_online",
                                "v1_0_0_world_3_level_8_scores_online",
                                "v1_0_0_world_3_level_9_scores_online",
                                "v1_0_0_world_3_level_10_scores_online",
                                "v1_0_0_world_3_level_11_scores_online",
                                "v1_0_0_world_3_level_12_scores_online",
                                "v1_0_0_world_3_level_13_scores_online",
                                "v1_0_0_world_3_level_14_scores_online",
                                "v1_0_0_world_3_level_15_scores_online",
                                "v1_0_0_world_3_level_16_scores_online",
                                "v1_0_0_world_3_level_17_scores_online",
                                "v1_0_0_world_3_level_18_scores_online",
                                "v1_0_0_world_3_level_19_scores_online",
                                "v1_0_0_world_3_level_20_scores_online",
                                "v1_0_0_world_3_level_21_scores_online"
                        },

                        {
                                "v1_0_0_world_4_level_1_scores_online",
                                "v1_0_0_world_4_level_2_scores_online",
                                "v1_0_0_world_4_level_3_scores_online",
                                "v1_0_0_world_4_level_4_scores_online",
                                "v1_0_0_world_4_level_5_scores_online",
                                "v1_0_0_world_4_level_6_scores_online",
                                "v1_0_0_world_4_level_7_scores_online",
                                "v1_0_0_world_4_level_8_scores_online",
                                "v1_0_0_world_4_level_9_scores_online",
                                "v1_0_0_world_4_level_10_scores_online",
                                "v1_0_0_world_4_level_11_scores_online",
                                "v1_0_0_world_4_level_12_scores_online",
                                "v1_0_0_world_4_level_13_scores_online",
                                "v1_0_0_world_4_level_14_scores_online",
                                "v1_0_0_world_4_level_15_scores_online",
                                "v1_0_0_world_4_level_16_scores_online",
                                "v1_0_0_world_4_level_17_scores_online",
                                "v1_0_0_world_4_level_18_scores_online",
                                "v1_0_0_world_4_level_19_scores_online",
                                "v1_0_0_world_4_level_20_scores_online",
                                "v1_0_0_world_4_level_21_scores_online"
                        },

                        {
                                "v1_0_0_world_5_level_1_scores_online",
                                "v1_0_0_world_5_level_2_scores_online",
                                "v1_0_0_world_5_level_3_scores_online",
                                "v1_0_0_world_5_level_4_scores_online",
                                "v1_0_0_world_5_level_5_scores_online",
                                "v1_0_0_world_5_level_6_scores_online",
                                "v1_0_0_world_5_level_7_scores_online",
                                "v1_0_0_world_5_level_8_scores_online",
                                "v1_0_0_world_5_level_9_scores_online",
                                "v1_0_0_world_5_level_10_scores_online",
                                "v1_0_0_world_5_level_11_scores_online",
                                "v1_0_0_world_5_level_12_scores_online",
                                "v1_0_0_world_5_level_13_scores_online",
                                "v1_0_0_world_5_level_14_scores_online",
                                "v1_0_0_world_5_level_15_scores_online",
                                "v1_0_0_world_5_level_16_scores_online",
                                "v1_0_0_world_5_level_17_scores_online",
                                "v1_0_0_world_5_level_18_scores_online",
                                "v1_0_0_world_5_level_19_scores_online",
                                "v1_0_0_world_5_level_20_scores_online",
                                "v1_0_0_world_5_level_21_scores_online"
                        }
                };
    }

    public static int getNumGoldenCarrots()
    {
        return _securePrefs.getInt(NUM_GOLDEN_CARROTS, 0);
    }

    public static void setNumGoldenCarrots(int numGoldenCarrots)
    {
        set(NUM_GOLDEN_CARROTS, numGoldenCarrots);
    }

    public static int getViewedCutscenesFlag()
    {
        return _securePrefs.getInt(VIEWED_CUTSCENES_FLAG, 0);
    }

    public static void setViewedCutscenesFlag(int jonViewedCutscenesFlag)
    {
        set(VIEWED_CUTSCENES_FLAG, jonViewedCutscenesFlag);
    }

    public static int getJonUnlockedAbilitiesFlag()
    {
        return _securePrefs.getInt(JON_UNLOCKED_ABILITIES_FLAG, 0);
    }

    public static int getLevelScore(int world, int level)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        return _securePrefs.getInt(WORLDS_LEVELS_SCORES[worldIndex][levelIndex], 0);
    }

    public static int getLevelStatsFlag(int world, int level)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        return _securePrefs.getInt(WORLDS_LEVELS_STATS[worldIndex][levelIndex], 0);
    }

    public static void setLevelStatsFlag(int world, int level, int levelStatsFlag)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        set(WORLDS_LEVELS_STATS[worldIndex][levelIndex], levelStatsFlag);
    }

    public static void setLevelComplete(int world, int level, int score, int levelStatsFlag, int jonUnlockedAbilitiesFlag)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        set(WORLDS_LEVELS_SCORES[worldIndex][levelIndex], score);
        set(WORLDS_LEVELS_STATS[worldIndex][levelIndex], levelStatsFlag);
        set(JON_UNLOCKED_ABILITIES_FLAG, jonUnlockedAbilitiesFlag);
    }

    public static int getScorePushedOnline(int world, int level)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        return _securePrefs.getInt(WORLDS_LEVELS_SCORES_ONLINE[worldIndex][levelIndex], 0);
    }

    public static void setScorePushedOnline(int world, int level, int score)
    {
        int worldIndex = world - 1;
        int levelIndex = level - 1;

        set(WORLDS_LEVELS_SCORES_ONLINE[worldIndex][levelIndex], score);
    }

    private static boolean set(String key, String value)
    {
        if (value == null)
        {
            return _securePrefs.edit().remove(key).commit();
        }

        return _securePrefs.edit().putString(key, value).commit();
    }

    private static boolean set(String key, float value)
    {
        return _securePrefs.edit().putFloat(key, value).commit();
    }

    private static boolean set(String key, long value)
    {
        return _securePrefs.edit().putLong(key, value).commit();
    }

    private static boolean set(String key, int value)
    {
        return _securePrefs.edit().putInt(key, value).commit();
    }

    private static boolean set(String key, boolean value)
    {
        return _securePrefs.edit().putBoolean(key, value).commit();
    }
}
