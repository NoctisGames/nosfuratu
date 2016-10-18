//
//  SaveData.m
//  nosfuratu
//
//  Created by Stephen Gowen on 3/11/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#import "SaveData.h"
#import "AppPrefs.h"

@implementation SaveData

static NSString *NUM_GOLDEN_CARROTS = @"v0_9_9_num_golden_carrots";
static NSString *JON_UNLOCKED_ABILITIES_FLAG = @"v0_9_9_jon_unlocked_abilities_flag";
static NSString *VIEWED_CUTSCENES_FLAG = @"v0_9_9_viewed_cutscenes_flag";
static NSArray *WORLDS_LEVELS_STATS;
static NSArray *WORLDS_LEVELS_SCORES;
static NSArray *WORLDS_LEVELS_SCORES_ONLINE;

+ (void)initialize
{
    WORLDS_LEVELS_STATS = [[NSArray alloc] initWithObjects:
                           [[NSArray alloc] initWithObjects:
                            @"v0_9_9_world_1_level_1_stats",
                            @"v0_9_9_world_1_level_2_stats",
                            @"v0_9_9_world_1_level_3_stats",
                            @"v0_9_9_world_1_level_4_stats",
                            @"v0_9_9_world_1_level_5_stats",
                            @"v0_9_9_world_1_level_6_stats",
                            @"v0_9_9_world_1_level_7_stats",
                            @"v0_9_9_world_1_level_8_stats",
                            @"v0_9_9_world_1_level_9_stats",
                            @"v0_9_9_world_1_level_10_stats",
                            @"v0_9_9_world_1_level_11_stats",
                            @"v0_9_9_world_1_level_12_stats",
                            @"v0_9_9_world_1_level_13_stats",
                            @"v0_9_9_world_1_level_14_stats",
                            @"v0_9_9_world_1_level_15_stats",
                            @"v0_9_9_world_1_level_16_stats",
                            @"v0_9_9_world_1_level_17_stats",
                            @"v0_9_9_world_1_level_18_stats",
                            @"v0_9_9_world_1_level_19_stats",
                            @"v0_9_9_world_1_level_20_stats",
                            @"v0_9_9_world_1_level_21_stats",
                            nil],
                           [[NSArray alloc] initWithObjects:
                            @"v0_9_9_world_2_level_1_stats",
                            @"v0_9_9_world_2_level_2_stats",
                            @"v0_9_9_world_2_level_3_stats",
                            @"v0_9_9_world_2_level_4_stats",
                            @"v0_9_9_world_2_level_5_stats",
                            @"v0_9_9_world_2_level_6_stats",
                            @"v0_9_9_world_2_level_7_stats",
                            @"v0_9_9_world_2_level_8_stats",
                            @"v0_9_9_world_2_level_9_stats",
                            @"v0_9_9_world_2_level_10_stats",
                            @"v0_9_9_world_2_level_11_stats",
                            @"v0_9_9_world_2_level_12_stats",
                            @"v0_9_9_world_2_level_13_stats",
                            @"v0_9_9_world_2_level_14_stats",
                            @"v0_9_9_world_2_level_15_stats",
                            @"v0_9_9_world_2_level_16_stats",
                            @"v0_9_9_world_2_level_17_stats",
                            @"v0_9_9_world_2_level_18_stats",
                            @"v0_9_9_world_2_level_19_stats",
                            @"v0_9_9_world_2_level_20_stats",
                            @"v0_9_9_world_2_level_21_stats",
                            nil],
                           [[NSArray alloc] initWithObjects:
                            @"v0_9_9_world_3_level_1_stats",
                            @"v0_9_9_world_3_level_2_stats",
                            @"v0_9_9_world_3_level_3_stats",
                            @"v0_9_9_world_3_level_4_stats",
                            @"v0_9_9_world_3_level_5_stats",
                            @"v0_9_9_world_3_level_6_stats",
                            @"v0_9_9_world_3_level_7_stats",
                            @"v0_9_9_world_3_level_8_stats",
                            @"v0_9_9_world_3_level_9_stats",
                            @"v0_9_9_world_3_level_10_stats",
                            @"v0_9_9_world_3_level_11_stats",
                            @"v0_9_9_world_3_level_12_stats",
                            @"v0_9_9_world_3_level_13_stats",
                            @"v0_9_9_world_3_level_14_stats",
                            @"v0_9_9_world_3_level_15_stats",
                            @"v0_9_9_world_3_level_16_stats",
                            @"v0_9_9_world_3_level_17_stats",
                            @"v0_9_9_world_3_level_18_stats",
                            @"v0_9_9_world_3_level_19_stats",
                            @"v0_9_9_world_3_level_20_stats",
                            @"v0_9_9_world_3_level_21_stats",
                            nil],
                           [[NSArray alloc] initWithObjects:
                            @"v0_9_9_world_4_level_1_stats",
                            @"v0_9_9_world_4_level_2_stats",
                            @"v0_9_9_world_4_level_3_stats",
                            @"v0_9_9_world_4_level_4_stats",
                            @"v0_9_9_world_4_level_5_stats",
                            @"v0_9_9_world_4_level_6_stats",
                            @"v0_9_9_world_4_level_7_stats",
                            @"v0_9_9_world_4_level_8_stats",
                            @"v0_9_9_world_4_level_9_stats",
                            @"v0_9_9_world_4_level_10_stats",
                            @"v0_9_9_world_4_level_11_stats",
                            @"v0_9_9_world_4_level_12_stats",
                            @"v0_9_9_world_4_level_13_stats",
                            @"v0_9_9_world_4_level_14_stats",
                            @"v0_9_9_world_4_level_15_stats",
                            @"v0_9_9_world_4_level_16_stats",
                            @"v0_9_9_world_4_level_17_stats",
                            @"v0_9_9_world_4_level_18_stats",
                            @"v0_9_9_world_4_level_19_stats",
                            @"v0_9_9_world_4_level_20_stats",
                            @"v0_9_9_world_4_level_21_stats",
                            nil],
                           [[NSArray alloc] initWithObjects:
                            @"v0_9_9_world_5_level_1_stats",
                            @"v0_9_9_world_5_level_2_stats",
                            @"v0_9_9_world_5_level_3_stats",
                            @"v0_9_9_world_5_level_4_stats",
                            @"v0_9_9_world_5_level_5_stats",
                            @"v0_9_9_world_5_level_6_stats",
                            @"v0_9_9_world_5_level_7_stats",
                            @"v0_9_9_world_5_level_8_stats",
                            @"v0_9_9_world_5_level_9_stats",
                            @"v0_9_9_world_5_level_10_stats",
                            @"v0_9_9_world_5_level_11_stats",
                            @"v0_9_9_world_5_level_12_stats",
                            @"v0_9_9_world_5_level_13_stats",
                            @"v0_9_9_world_5_level_14_stats",
                            @"v0_9_9_world_5_level_15_stats",
                            @"v0_9_9_world_5_level_16_stats",
                            @"v0_9_9_world_5_level_17_stats",
                            @"v0_9_9_world_5_level_18_stats",
                            @"v0_9_9_world_5_level_19_stats",
                            @"v0_9_9_world_5_level_20_stats",
                            @"v0_9_9_world_5_level_21_stats",
                            nil],
                           nil];
    
    WORLDS_LEVELS_SCORES = [[NSArray alloc] initWithObjects:
                            [[NSArray alloc] initWithObjects:
                             @"v0_9_9_world_1_level_1_scores",
                             @"v0_9_9_world_1_level_2_scores",
                             @"v0_9_9_world_1_level_3_scores",
                             @"v0_9_9_world_1_level_4_scores",
                             @"v0_9_9_world_1_level_5_scores",
                             @"v0_9_9_world_1_level_6_scores",
                             @"v0_9_9_world_1_level_7_scores",
                             @"v0_9_9_world_1_level_8_scores",
                             @"v0_9_9_world_1_level_9_scores",
                             @"v0_9_9_world_1_level_10_scores",
                             @"v0_9_9_world_1_level_11_scores",
                             @"v0_9_9_world_1_level_12_scores",
                             @"v0_9_9_world_1_level_13_scores",
                             @"v0_9_9_world_1_level_14_scores",
                             @"v0_9_9_world_1_level_15_scores",
                             @"v0_9_9_world_1_level_16_scores",
                             @"v0_9_9_world_1_level_17_scores",
                             @"v0_9_9_world_1_level_18_scores",
                             @"v0_9_9_world_1_level_19_scores",
                             @"v0_9_9_world_1_level_20_scores",
                             @"v0_9_9_world_1_level_21_scores",
                             nil],
                            [[NSArray alloc] initWithObjects:
                             @"v0_9_9_world_2_level_1_scores",
                             @"v0_9_9_world_2_level_2_scores",
                             @"v0_9_9_world_2_level_3_scores",
                             @"v0_9_9_world_2_level_4_scores",
                             @"v0_9_9_world_2_level_5_scores",
                             @"v0_9_9_world_2_level_6_scores",
                             @"v0_9_9_world_2_level_7_scores",
                             @"v0_9_9_world_2_level_8_scores",
                             @"v0_9_9_world_2_level_9_scores",
                             @"v0_9_9_world_2_level_10_scores",
                             @"v0_9_9_world_2_level_11_scores",
                             @"v0_9_9_world_2_level_12_scores",
                             @"v0_9_9_world_2_level_13_scores",
                             @"v0_9_9_world_2_level_14_scores",
                             @"v0_9_9_world_2_level_15_scores",
                             @"v0_9_9_world_2_level_16_scores",
                             @"v0_9_9_world_2_level_17_scores",
                             @"v0_9_9_world_2_level_18_scores",
                             @"v0_9_9_world_2_level_19_scores",
                             @"v0_9_9_world_2_level_20_scores",
                             @"v0_9_9_world_2_level_21_scores",
                             nil],
                            [[NSArray alloc] initWithObjects:
                             @"v0_9_9_world_3_level_1_scores",
                             @"v0_9_9_world_3_level_2_scores",
                             @"v0_9_9_world_3_level_3_scores",
                             @"v0_9_9_world_3_level_4_scores",
                             @"v0_9_9_world_3_level_5_scores",
                             @"v0_9_9_world_3_level_6_scores",
                             @"v0_9_9_world_3_level_7_scores",
                             @"v0_9_9_world_3_level_8_scores",
                             @"v0_9_9_world_3_level_9_scores",
                             @"v0_9_9_world_3_level_10_scores",
                             @"v0_9_9_world_3_level_11_scores",
                             @"v0_9_9_world_3_level_12_scores",
                             @"v0_9_9_world_3_level_13_scores",
                             @"v0_9_9_world_3_level_14_scores",
                             @"v0_9_9_world_3_level_15_scores",
                             @"v0_9_9_world_3_level_16_scores",
                             @"v0_9_9_world_3_level_17_scores",
                             @"v0_9_9_world_3_level_18_scores",
                             @"v0_9_9_world_3_level_19_scores",
                             @"v0_9_9_world_3_level_20_scores",
                             @"v0_9_9_world_3_level_21_scores",
                             nil],
                            [[NSArray alloc] initWithObjects:
                             @"v0_9_9_world_4_level_1_scores",
                             @"v0_9_9_world_4_level_2_scores",
                             @"v0_9_9_world_4_level_3_scores",
                             @"v0_9_9_world_4_level_4_scores",
                             @"v0_9_9_world_4_level_5_scores",
                             @"v0_9_9_world_4_level_6_scores",
                             @"v0_9_9_world_4_level_7_scores",
                             @"v0_9_9_world_4_level_8_scores",
                             @"v0_9_9_world_4_level_9_scores",
                             @"v0_9_9_world_4_level_10_scores",
                             @"v0_9_9_world_4_level_11_scores",
                             @"v0_9_9_world_4_level_12_scores",
                             @"v0_9_9_world_4_level_13_scores",
                             @"v0_9_9_world_4_level_14_scores",
                             @"v0_9_9_world_4_level_15_scores",
                             @"v0_9_9_world_4_level_16_scores",
                             @"v0_9_9_world_4_level_17_scores",
                             @"v0_9_9_world_4_level_18_scores",
                             @"v0_9_9_world_4_level_19_scores",
                             @"v0_9_9_world_4_level_20_scores",
                             @"v0_9_9_world_4_level_21_scores",
                             nil],
                            [[NSArray alloc] initWithObjects:
                             @"v0_9_9_world_5_level_1_scores",
                             @"v0_9_9_world_5_level_2_scores",
                             @"v0_9_9_world_5_level_3_scores",
                             @"v0_9_9_world_5_level_4_scores",
                             @"v0_9_9_world_5_level_5_scores",
                             @"v0_9_9_world_5_level_6_scores",
                             @"v0_9_9_world_5_level_7_scores",
                             @"v0_9_9_world_5_level_8_scores",
                             @"v0_9_9_world_5_level_9_scores",
                             @"v0_9_9_world_5_level_10_scores",
                             @"v0_9_9_world_5_level_11_scores",
                             @"v0_9_9_world_5_level_12_scores",
                             @"v0_9_9_world_5_level_13_scores",
                             @"v0_9_9_world_5_level_14_scores",
                             @"v0_9_9_world_5_level_15_scores",
                             @"v0_9_9_world_5_level_16_scores",
                             @"v0_9_9_world_5_level_17_scores",
                             @"v0_9_9_world_5_level_18_scores",
                             @"v0_9_9_world_5_level_19_scores",
                             @"v0_9_9_world_5_level_20_scores",
                             @"v0_9_9_world_5_level_21_scores",
                             nil],
                            nil];
    
    WORLDS_LEVELS_SCORES_ONLINE = [[NSArray alloc] initWithObjects:
                                   [[NSArray alloc] initWithObjects:
                                    @"v0_9_9_world_1_level_1_scores_online",
                                    @"v0_9_9_world_1_level_2_scores_online",
                                    @"v0_9_9_world_1_level_3_scores_online",
                                    @"v0_9_9_world_1_level_4_scores_online",
                                    @"v0_9_9_world_1_level_5_scores_online",
                                    @"v0_9_9_world_1_level_6_scores_online",
                                    @"v0_9_9_world_1_level_7_scores_online",
                                    @"v0_9_9_world_1_level_8_scores_online",
                                    @"v0_9_9_world_1_level_9_scores_online",
                                    @"v0_9_9_world_1_level_10_scores_online",
                                    @"v0_9_9_world_1_level_11_scores_online",
                                    @"v0_9_9_world_1_level_12_scores_online",
                                    @"v0_9_9_world_1_level_13_scores_online",
                                    @"v0_9_9_world_1_level_14_scores_online",
                                    @"v0_9_9_world_1_level_15_scores_online",
                                    @"v0_9_9_world_1_level_16_scores_online",
                                    @"v0_9_9_world_1_level_17_scores_online",
                                    @"v0_9_9_world_1_level_18_scores_online",
                                    @"v0_9_9_world_1_level_19_scores_online",
                                    @"v0_9_9_world_1_level_20_scores_online",
                                    @"v0_9_9_world_1_level_21_scores_online",
                                    nil],
                                   [[NSArray alloc] initWithObjects:
                                    @"v0_9_9_world_2_level_1_scores_online",
                                    @"v0_9_9_world_2_level_2_scores_online",
                                    @"v0_9_9_world_2_level_3_scores_online",
                                    @"v0_9_9_world_2_level_4_scores_online",
                                    @"v0_9_9_world_2_level_5_scores_online",
                                    @"v0_9_9_world_2_level_6_scores_online",
                                    @"v0_9_9_world_2_level_7_scores_online",
                                    @"v0_9_9_world_2_level_8_scores_online",
                                    @"v0_9_9_world_2_level_9_scores_online",
                                    @"v0_9_9_world_2_level_10_scores_online",
                                    @"v0_9_9_world_2_level_11_scores_online",
                                    @"v0_9_9_world_2_level_12_scores_online",
                                    @"v0_9_9_world_2_level_13_scores_online",
                                    @"v0_9_9_world_2_level_14_scores_online",
                                    @"v0_9_9_world_2_level_15_scores_online",
                                    @"v0_9_9_world_2_level_16_scores_online",
                                    @"v0_9_9_world_2_level_17_scores_online",
                                    @"v0_9_9_world_2_level_18_scores_online",
                                    @"v0_9_9_world_2_level_19_scores_online",
                                    @"v0_9_9_world_2_level_20_scores_online",
                                    @"v0_9_9_world_2_level_21_scores_online",
                                    nil],
                                   [[NSArray alloc] initWithObjects:
                                    @"v0_9_9_world_3_level_1_scores_online",
                                    @"v0_9_9_world_3_level_2_scores_online",
                                    @"v0_9_9_world_3_level_3_scores_online",
                                    @"v0_9_9_world_3_level_4_scores_online",
                                    @"v0_9_9_world_3_level_5_scores_online",
                                    @"v0_9_9_world_3_level_6_scores_online",
                                    @"v0_9_9_world_3_level_7_scores_online",
                                    @"v0_9_9_world_3_level_8_scores_online",
                                    @"v0_9_9_world_3_level_9_scores_online",
                                    @"v0_9_9_world_3_level_10_scores_online",
                                    @"v0_9_9_world_3_level_11_scores_online",
                                    @"v0_9_9_world_3_level_12_scores_online",
                                    @"v0_9_9_world_3_level_13_scores_online",
                                    @"v0_9_9_world_3_level_14_scores_online",
                                    @"v0_9_9_world_3_level_15_scores_online",
                                    @"v0_9_9_world_3_level_16_scores_online",
                                    @"v0_9_9_world_3_level_17_scores_online",
                                    @"v0_9_9_world_3_level_18_scores_online",
                                    @"v0_9_9_world_3_level_19_scores_online",
                                    @"v0_9_9_world_3_level_20_scores_online",
                                    @"v0_9_9_world_3_level_21_scores_online",
                                    nil],
                                   [[NSArray alloc] initWithObjects:
                                    @"v0_9_9_world_4_level_1_scores_online",
                                    @"v0_9_9_world_4_level_2_scores_online",
                                    @"v0_9_9_world_4_level_3_scores_online",
                                    @"v0_9_9_world_4_level_4_scores_online",
                                    @"v0_9_9_world_4_level_5_scores_online",
                                    @"v0_9_9_world_4_level_6_scores_online",
                                    @"v0_9_9_world_4_level_7_scores_online",
                                    @"v0_9_9_world_4_level_8_scores_online",
                                    @"v0_9_9_world_4_level_9_scores_online",
                                    @"v0_9_9_world_4_level_10_scores_online",
                                    @"v0_9_9_world_4_level_11_scores_online",
                                    @"v0_9_9_world_4_level_12_scores_online",
                                    @"v0_9_9_world_4_level_13_scores_online",
                                    @"v0_9_9_world_4_level_14_scores_online",
                                    @"v0_9_9_world_4_level_15_scores_online",
                                    @"v0_9_9_world_4_level_16_scores_online",
                                    @"v0_9_9_world_4_level_17_scores_online",
                                    @"v0_9_9_world_4_level_18_scores_online",
                                    @"v0_9_9_world_4_level_19_scores_online",
                                    @"v0_9_9_world_4_level_20_scores_online",
                                    @"v0_9_9_world_4_level_21_scores_online",
                                    nil],
                                   [[NSArray alloc] initWithObjects:
                                    @"v0_9_9_world_5_level_1_scores_online",
                                    @"v0_9_9_world_5_level_2_scores_online",
                                    @"v0_9_9_world_5_level_3_scores_online",
                                    @"v0_9_9_world_5_level_4_scores_online",
                                    @"v0_9_9_world_5_level_5_scores_online",
                                    @"v0_9_9_world_5_level_6_scores_online",
                                    @"v0_9_9_world_5_level_7_scores_online",
                                    @"v0_9_9_world_5_level_8_scores_online",
                                    @"v0_9_9_world_5_level_9_scores_online",
                                    @"v0_9_9_world_5_level_10_scores_online",
                                    @"v0_9_9_world_5_level_11_scores_online",
                                    @"v0_9_9_world_5_level_12_scores_online",
                                    @"v0_9_9_world_5_level_13_scores_online",
                                    @"v0_9_9_world_5_level_14_scores_online",
                                    @"v0_9_9_world_5_level_15_scores_online",
                                    @"v0_9_9_world_5_level_16_scores_online",
                                    @"v0_9_9_world_5_level_17_scores_online",
                                    @"v0_9_9_world_5_level_18_scores_online",
                                    @"v0_9_9_world_5_level_19_scores_online",
                                    @"v0_9_9_world_5_level_20_scores_online",
                                    @"v0_9_9_world_5_level_21_scores_online",
                                    nil],
                                   nil];
}

+ (int)getNumGoldenCarrots
{
    return [[AppPrefs getInstance] getInt:NUM_GOLDEN_CARROTS];
}

+ (void)setNumGoldenCarrots:(int)numGoldenCarrots
{
    [[AppPrefs getInstance] setInt:numGoldenCarrots forKey:NUM_GOLDEN_CARROTS];
}

+ (int)getViewedCutscenesFlag
{
    return [[AppPrefs getInstance] getInt:VIEWED_CUTSCENES_FLAG];
}

+ (void)setViewedCutscenesFlag:(int)viewedCutscenesFlag
{
    [[AppPrefs getInstance] setInt:viewedCutscenesFlag forKey:VIEWED_CUTSCENES_FLAG];
}

+ (int)getJonUnlockedAbilitiesFlag
{
    return [[AppPrefs getInstance] getInt:JON_UNLOCKED_ABILITIES_FLAG];
}

+ (int)getLevelScore:(int)world level:(int)level
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    return [[AppPrefs getInstance] getInt:WORLDS_LEVELS_SCORES[worldIndex][levelIndex]];
}

+ (int)getLevelStatsFlag:(int)world level:(int)level
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    return [[AppPrefs getInstance] getInt:WORLDS_LEVELS_STATS[worldIndex][levelIndex]];
}

+ (void)setLevelStatsFlag:(int)world level:(int)level levelStatsFlag:(int)levelStatsFlag
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    [[AppPrefs getInstance] setInt:levelStatsFlag forKey:WORLDS_LEVELS_STATS[worldIndex][levelIndex]];
}

+ (void)setLevelComplete:(int)world level:(int)level score:(int)score levelStatsFlag:(int)levelStatsFlag jonUnlockedAbilitiesFlag:(int)jonUnlockedAbilitiesFlag
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    [[AppPrefs getInstance] setInt:score forKey:WORLDS_LEVELS_SCORES[worldIndex][levelIndex]];
    [[AppPrefs getInstance] setInt:levelStatsFlag forKey:WORLDS_LEVELS_STATS[worldIndex][levelIndex]];
    [[AppPrefs getInstance] setInt:jonUnlockedAbilitiesFlag forKey:JON_UNLOCKED_ABILITIES_FLAG];
}

+ (int)getScorePushedOnline:(int)world level:(int)level
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    return [[AppPrefs getInstance] getInt:WORLDS_LEVELS_SCORES_ONLINE[worldIndex][levelIndex]];
}

+ (void)setScorePushedOnline:(int)world level:(int)level score:(int)score
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    [[AppPrefs getInstance] setInt:score forKey:WORLDS_LEVELS_SCORES_ONLINE[worldIndex][levelIndex]];
}

@end
