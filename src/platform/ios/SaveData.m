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

static NSArray *WORLDS_LEVELS_COMPLETION;

+ (void)initialize
{
    WORLDS_LEVELS_COMPLETION = [[NSArray alloc] initWithObjects:
                                [[NSArray alloc] initWithObjects:
                                 @"is_world_1_level_1_complete",
                                 @"is_world_1_level_2_complete",
                                 @"is_world_1_level_3_complete",
                                 @"is_world_1_level_4_complete",
                                 @"is_world_1_level_5_complete",
                                 @"is_world_1_level_6_complete",
                                 @"is_world_1_level_7_complete",
                                 @"is_world_1_level_8_complete",
                                 @"is_world_1_level_9_complete",
                                 @"is_world_1_level_10_complete",
                                 @"is_world_1_level_11_complete",
                                 @"is_world_1_level_12_complete",
                                 @"is_world_1_level_13_complete",
                                 @"is_world_1_level_14_complete",
                                 @"is_world_1_level_15_complete",
                                 @"is_world_1_level_16_complete",
                                 @"is_world_1_level_17_complete",
                                 @"is_world_1_level_18_complete",
                                 @"is_world_1_level_19_complete",
                                 @"is_world_1_level_20_complete",
                                 @"is_world_1_level_21_complete",
                                 nil],
                                [[NSArray alloc] initWithObjects:
                                 @"is_world_2_level_1_complete",
                                 @"is_world_2_level_2_complete",
                                 @"is_world_2_level_3_complete",
                                 @"is_world_2_level_4_complete",
                                 @"is_world_2_level_5_complete",
                                 @"is_world_2_level_6_complete",
                                 @"is_world_2_level_7_complete",
                                 @"is_world_2_level_8_complete",
                                 @"is_world_2_level_9_complete",
                                 @"is_world_2_level_10_complete",
                                 @"is_world_2_level_11_complete",
                                 @"is_world_2_level_12_complete",
                                 @"is_world_2_level_13_complete",
                                 @"is_world_2_level_14_complete",
                                 @"is_world_2_level_15_complete",
                                 @"is_world_2_level_16_complete",
                                 @"is_world_2_level_17_complete",
                                 @"is_world_2_level_18_complete",
                                 @"is_world_2_level_19_complete",
                                 @"is_world_2_level_20_complete",
                                 @"is_world_2_level_21_complete",
                                 nil],
                                [[NSArray alloc] initWithObjects:
                                 @"is_world_3_level_1_complete",
                                 @"is_world_3_level_2_complete",
                                 @"is_world_3_level_3_complete",
                                 @"is_world_3_level_4_complete",
                                 @"is_world_3_level_5_complete",
                                 @"is_world_3_level_6_complete",
                                 @"is_world_3_level_7_complete",
                                 @"is_world_3_level_8_complete",
                                 @"is_world_3_level_9_complete",
                                 @"is_world_3_level_10_complete",
                                 @"is_world_3_level_11_complete",
                                 @"is_world_3_level_12_complete",
                                 @"is_world_3_level_13_complete",
                                 @"is_world_3_level_14_complete",
                                 @"is_world_3_level_15_complete",
                                 @"is_world_3_level_16_complete",
                                 @"is_world_3_level_17_complete",
                                 @"is_world_3_level_18_complete",
                                 @"is_world_3_level_19_complete",
                                 @"is_world_3_level_20_complete",
                                 @"is_world_3_level_21_complete",
                                 nil],
                                [[NSArray alloc] initWithObjects:
                                 @"is_world_4_level_1_complete",
                                 @"is_world_4_level_2_complete",
                                 @"is_world_4_level_3_complete",
                                 @"is_world_4_level_4_complete",
                                 @"is_world_4_level_5_complete",
                                 @"is_world_4_level_6_complete",
                                 @"is_world_4_level_7_complete",
                                 @"is_world_4_level_8_complete",
                                 @"is_world_4_level_9_complete",
                                 @"is_world_4_level_10_complete",
                                 @"is_world_4_level_11_complete",
                                 @"is_world_4_level_12_complete",
                                 @"is_world_4_level_13_complete",
                                 @"is_world_4_level_14_complete",
                                 @"is_world_4_level_15_complete",
                                 @"is_world_4_level_16_complete",
                                 @"is_world_4_level_17_complete",
                                 @"is_world_4_level_18_complete",
                                 @"is_world_4_level_19_complete",
                                 @"is_world_4_level_20_complete",
                                 @"is_world_4_level_21_complete",
                                 nil],
                                [[NSArray alloc] initWithObjects:
                                 @"is_world_5_level_1_complete",
                                 @"is_world_5_level_2_complete",
                                 @"is_world_5_level_3_complete",
                                 @"is_world_5_level_4_complete",
                                 @"is_world_5_level_5_complete",
                                 @"is_world_5_level_6_complete",
                                 @"is_world_5_level_7_complete",
                                 @"is_world_5_level_8_complete",
                                 @"is_world_5_level_9_complete",
                                 @"is_world_5_level_10_complete",
                                 @"is_world_5_level_11_complete",
                                 @"is_world_5_level_12_complete",
                                 @"is_world_5_level_13_complete",
                                 @"is_world_5_level_14_complete",
                                 @"is_world_5_level_15_complete",
                                 @"is_world_5_level_16_complete",
                                 @"is_world_5_level_17_complete",
                                 @"is_world_5_level_18_complete",
                                 @"is_world_5_level_19_complete",
                                 @"is_world_5_level_20_complete",
                                 @"is_world_5_level_21_complete",
                                 nil],
                                 nil];
}


+ (BOOL)isLevelComplete:(int)world level:(int)level
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    return [[AppPrefs getInstance] getBool:WORLDS_LEVELS_COMPLETION[worldIndex][levelIndex]];
}

+ (void)setLevelComplete:(int)world level:(int)level
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    [[AppPrefs getInstance] setBool:YES forKey:WORLDS_LEVELS_COMPLETION[worldIndex][levelIndex]];
}

@end
