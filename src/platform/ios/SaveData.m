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

static NSArray *WORLDS_LEVELS_STATS;

+ (void)initialize
{
    WORLDS_LEVELS_STATS = [[NSArray alloc] initWithObjects:
                                [[NSArray alloc] initWithObjects:
                                 @"world_1_level_1_stats",
                                 @"world_1_level_2_stats",
                                 @"world_1_level_3_stats",
                                 @"world_1_level_4_stats",
                                 @"world_1_level_5_stats",
                                 @"world_1_level_6_stats",
                                 @"world_1_level_7_stats",
                                 @"world_1_level_8_stats",
                                 @"world_1_level_9_stats",
                                 @"world_1_level_10_stats",
                                 @"world_1_level_11_stats",
                                 @"world_1_level_12_stats",
                                 @"world_1_level_13_stats",
                                 @"world_1_level_14_stats",
                                 @"world_1_level_15_stats",
                                 @"world_1_level_16_stats",
                                 @"world_1_level_17_stats",
                                 @"world_1_level_18_stats",
                                 @"world_1_level_19_stats",
                                 @"world_1_level_20_stats",
                                 @"world_1_level_21_stats",
                                 nil],
                                [[NSArray alloc] initWithObjects:
                                 @"world_2_level_1_stats",
                                 @"world_2_level_2_stats",
                                 @"world_2_level_3_stats",
                                 @"world_2_level_4_stats",
                                 @"world_2_level_5_stats",
                                 @"world_2_level_6_stats",
                                 @"world_2_level_7_stats",
                                 @"world_2_level_8_stats",
                                 @"world_2_level_9_stats",
                                 @"world_2_level_10_stats",
                                 @"world_2_level_11_stats",
                                 @"world_2_level_12_stats",
                                 @"world_2_level_13_stats",
                                 @"world_2_level_14_stats",
                                 @"world_2_level_15_stats",
                                 @"world_2_level_16_stats",
                                 @"world_2_level_17_stats",
                                 @"world_2_level_18_stats",
                                 @"world_2_level_19_stats",
                                 @"world_2_level_20_stats",
                                 @"world_2_level_21_stats",
                                 nil],
                                [[NSArray alloc] initWithObjects:
                                 @"world_3_level_1_stats",
                                 @"world_3_level_2_stats",
                                 @"world_3_level_3_stats",
                                 @"world_3_level_4_stats",
                                 @"world_3_level_5_stats",
                                 @"world_3_level_6_stats",
                                 @"world_3_level_7_stats",
                                 @"world_3_level_8_stats",
                                 @"world_3_level_9_stats",
                                 @"world_3_level_10_stats",
                                 @"world_3_level_11_stats",
                                 @"world_3_level_12_stats",
                                 @"world_3_level_13_stats",
                                 @"world_3_level_14_stats",
                                 @"world_3_level_15_stats",
                                 @"world_3_level_16_stats",
                                 @"world_3_level_17_stats",
                                 @"world_3_level_18_stats",
                                 @"world_3_level_19_stats",
                                 @"world_3_level_20_stats",
                                 @"world_3_level_21_stats",
                                 nil],
                                [[NSArray alloc] initWithObjects:
                                 @"world_4_level_1_stats",
                                 @"world_4_level_2_stats",
                                 @"world_4_level_3_stats",
                                 @"world_4_level_4_stats",
                                 @"world_4_level_5_stats",
                                 @"world_4_level_6_stats",
                                 @"world_4_level_7_stats",
                                 @"world_4_level_8_stats",
                                 @"world_4_level_9_stats",
                                 @"world_4_level_10_stats",
                                 @"world_4_level_11_stats",
                                 @"world_4_level_12_stats",
                                 @"world_4_level_13_stats",
                                 @"world_4_level_14_stats",
                                 @"world_4_level_15_stats",
                                 @"world_4_level_16_stats",
                                 @"world_4_level_17_stats",
                                 @"world_4_level_18_stats",
                                 @"world_4_level_19_stats",
                                 @"world_4_level_20_stats",
                                 @"world_4_level_21_stats",
                                 nil],
                                [[NSArray alloc] initWithObjects:
                                 @"world_5_level_1_stats",
                                 @"world_5_level_2_stats",
                                 @"world_5_level_3_stats",
                                 @"world_5_level_4_stats",
                                 @"world_5_level_5_stats",
                                 @"world_5_level_6_stats",
                                 @"world_5_level_7_stats",
                                 @"world_5_level_8_stats",
                                 @"world_5_level_9_stats",
                                 @"world_5_level_10_stats",
                                 @"world_5_level_11_stats",
                                 @"world_5_level_12_stats",
                                 @"world_5_level_13_stats",
                                 @"world_5_level_14_stats",
                                 @"world_5_level_15_stats",
                                 @"world_5_level_16_stats",
                                 @"world_5_level_17_stats",
                                 @"world_5_level_18_stats",
                                 @"world_5_level_19_stats",
                                 @"world_5_level_20_stats",
                                 @"world_5_level_21_stats",
                                 nil],
                                 nil];
}


+ (int)getLevelStats:(int)world level:(int)level
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    return [[AppPrefs getInstance] getInt:WORLDS_LEVELS_STATS[worldIndex][levelIndex]];
}

+ (void)setLevelComplete:(int)world level:(int)level goldenCarrotsFlag:(int)gcf
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    [[AppPrefs getInstance] setInt:gcf forKey:WORLDS_LEVELS_STATS[worldIndex][levelIndex]];
}

@end
