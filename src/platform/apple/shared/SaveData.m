//
//  SaveData.m
//  nosfuratu
//
//  Created by Stephen Gowen on 3/11/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#import "SaveData.h"

#import "NSUserDefaults+SecureAdditions.h"

@implementation SaveData

static NSString *NUM_GOLDEN_CARROTS = @"v1_0_0_num_golden_carrots";
static NSString *JON_UNLOCKED_ABILITIES_FLAG = @"v1_0_0_jon_unlocked_abilities_flag";
static NSString *VIEWED_CUTSCENES_FLAG = @"v1_0_0_viewed_cutscenes_flag";
static NSArray *WORLDS_LEVELS_STATS;
static NSArray *WORLDS_LEVELS_SCORES;
static NSArray *WORLDS_LEVELS_SCORES_ONLINE;

+ (void)initialize
{
    [[NSUserDefaults standardUserDefaults] setSecret:@"noctis_games_nosfuratu"];
    
    WORLDS_LEVELS_STATS = [[NSArray alloc] initWithObjects:
                           [[NSArray alloc] initWithObjects:
                            @"v1_0_0_world_1_level_1_stats",
                            @"v1_0_0_world_1_level_2_stats",
                            @"v1_0_0_world_1_level_3_stats",
                            @"v1_0_0_world_1_level_4_stats",
                            @"v1_0_0_world_1_level_5_stats",
                            @"v1_0_0_world_1_level_6_stats",
                            @"v1_0_0_world_1_level_7_stats",
                            @"v1_0_0_world_1_level_8_stats",
                            @"v1_0_0_world_1_level_9_stats",
                            @"v1_0_0_world_1_level_10_stats",
                            @"v1_0_0_world_1_level_11_stats",
                            @"v1_0_0_world_1_level_12_stats",
                            @"v1_0_0_world_1_level_13_stats",
                            @"v1_0_0_world_1_level_14_stats",
                            @"v1_0_0_world_1_level_15_stats",
                            @"v1_0_0_world_1_level_16_stats",
                            @"v1_0_0_world_1_level_17_stats",
                            @"v1_0_0_world_1_level_18_stats",
                            @"v1_0_0_world_1_level_19_stats",
                            @"v1_0_0_world_1_level_20_stats",
                            @"v1_0_0_world_1_level_21_stats",
                            nil],
                           nil];
    
    WORLDS_LEVELS_SCORES = [[NSArray alloc] initWithObjects:
                            [[NSArray alloc] initWithObjects:
                             @"v1_0_0_world_1_level_1_scores",
                             @"v1_0_0_world_1_level_2_scores",
                             @"v1_0_0_world_1_level_3_scores",
                             @"v1_0_0_world_1_level_4_scores",
                             @"v1_0_0_world_1_level_5_scores",
                             @"v1_0_0_world_1_level_6_scores",
                             @"v1_0_0_world_1_level_7_scores",
                             @"v1_0_0_world_1_level_8_scores",
                             @"v1_0_0_world_1_level_9_scores",
                             @"v1_0_0_world_1_level_10_scores",
                             @"v1_0_0_world_1_level_11_scores",
                             @"v1_0_0_world_1_level_12_scores",
                             @"v1_0_0_world_1_level_13_scores",
                             @"v1_0_0_world_1_level_14_scores",
                             @"v1_0_0_world_1_level_15_scores",
                             @"v1_0_0_world_1_level_16_scores",
                             @"v1_0_0_world_1_level_17_scores",
                             @"v1_0_0_world_1_level_18_scores",
                             @"v1_0_0_world_1_level_19_scores",
                             @"v1_0_0_world_1_level_20_scores",
                             @"v1_0_0_world_1_level_21_scores",
                             nil],
                            nil];
    
    WORLDS_LEVELS_SCORES_ONLINE = [[NSArray alloc] initWithObjects:
                                   [[NSArray alloc] initWithObjects:
                                    @"v1_0_0_world_1_level_1_scores_online",
                                    @"v1_0_0_world_1_level_2_scores_online",
                                    @"v1_0_0_world_1_level_3_scores_online",
                                    @"v1_0_0_world_1_level_4_scores_online",
                                    @"v1_0_0_world_1_level_5_scores_online",
                                    @"v1_0_0_world_1_level_6_scores_online",
                                    @"v1_0_0_world_1_level_7_scores_online",
                                    @"v1_0_0_world_1_level_8_scores_online",
                                    @"v1_0_0_world_1_level_9_scores_online",
                                    @"v1_0_0_world_1_level_10_scores_online",
                                    @"v1_0_0_world_1_level_11_scores_online",
                                    @"v1_0_0_world_1_level_12_scores_online",
                                    @"v1_0_0_world_1_level_13_scores_online",
                                    @"v1_0_0_world_1_level_14_scores_online",
                                    @"v1_0_0_world_1_level_15_scores_online",
                                    @"v1_0_0_world_1_level_16_scores_online",
                                    @"v1_0_0_world_1_level_17_scores_online",
                                    @"v1_0_0_world_1_level_18_scores_online",
                                    @"v1_0_0_world_1_level_19_scores_online",
                                    @"v1_0_0_world_1_level_20_scores_online",
                                    @"v1_0_0_world_1_level_21_scores_online",
                                    nil],
                                   nil];
}

+ (int)getNumGoldenCarrots
{
    return (int) [[NSUserDefaults standardUserDefaults] secretIntegerForKey:NUM_GOLDEN_CARROTS];
}

+ (void)setNumGoldenCarrots:(int)numGoldenCarrots
{
    [[NSUserDefaults standardUserDefaults] setSecretInteger:numGoldenCarrots forKey:NUM_GOLDEN_CARROTS];
}

+ (int)getViewedCutscenesFlag
{
    return (int) [[NSUserDefaults standardUserDefaults] secretIntegerForKey:VIEWED_CUTSCENES_FLAG];
}

+ (void)setViewedCutscenesFlag:(int)viewedCutscenesFlag
{
    [[NSUserDefaults standardUserDefaults] setSecretInteger:viewedCutscenesFlag forKey:VIEWED_CUTSCENES_FLAG];
}

+ (int)getJonUnlockedAbilitiesFlag
{
    return (int) [[NSUserDefaults standardUserDefaults] secretIntegerForKey:JON_UNLOCKED_ABILITIES_FLAG];
}

+ (int)getLevelScore:(int)world level:(int)level
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    return (int) [[NSUserDefaults standardUserDefaults] secretIntegerForKey:WORLDS_LEVELS_SCORES[worldIndex][levelIndex]];
}

+ (int)getLevelStatsFlag:(int)world level:(int)level
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    return (int) [[NSUserDefaults standardUserDefaults] secretIntegerForKey:WORLDS_LEVELS_STATS[worldIndex][levelIndex]];
}

+ (void)setLevelStatsFlag:(int)world level:(int)level levelStatsFlag:(int)levelStatsFlag
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    [[NSUserDefaults standardUserDefaults] setSecretInteger:levelStatsFlag forKey:WORLDS_LEVELS_STATS[worldIndex][levelIndex]];
}

+ (void)setLevelComplete:(int)world level:(int)level score:(int)score levelStatsFlag:(int)levelStatsFlag jonUnlockedAbilitiesFlag:(int)jonUnlockedAbilitiesFlag
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    [[NSUserDefaults standardUserDefaults] setSecretInteger:score forKey:WORLDS_LEVELS_SCORES[worldIndex][levelIndex]];
    [[NSUserDefaults standardUserDefaults] setSecretInteger:levelStatsFlag forKey:WORLDS_LEVELS_STATS[worldIndex][levelIndex]];
    [[NSUserDefaults standardUserDefaults] setSecretInteger:jonUnlockedAbilitiesFlag forKey:JON_UNLOCKED_ABILITIES_FLAG];
}

+ (int)getScorePushedOnline:(int)world level:(int)level
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    return (int) [[NSUserDefaults standardUserDefaults] secretIntegerForKey:WORLDS_LEVELS_SCORES_ONLINE[worldIndex][levelIndex]];
}

+ (void)setScorePushedOnline:(int)world level:(int)level score:(int)score
{
    int worldIndex = world - 1;
    int levelIndex = level - 1;
    
    [[NSUserDefaults standardUserDefaults] setSecretInteger:score forKey:WORLDS_LEVELS_SCORES_ONLINE[worldIndex][levelIndex]];
}

@end
