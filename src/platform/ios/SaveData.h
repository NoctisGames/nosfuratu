//
//  SaveData.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/11/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

@interface SaveData : NSObject
{
    // Empty
}

+ (int)getNumGoldenCarrots;
+ (void)setNumGoldenCarrots:(int)numGoldenCarrots;

+ (int)getViewedCutscenesFlag;
+ (void)setViewedCutscenesFlag:(int)viewedCutscenesFlag;

+ (int)getJonUnlockedAbilitiesFlag;

+ (int)getLevelScore:(int)world level:(int)level;

+ (int)getLevelStatsFlag:(int)world level:(int)level;
+ (void)setLevelStatsFlag:(int)world level:(int)level levelStatsFlag:(int)levelStatsFlag;

+ (void)setLevelComplete:(int)world level:(int)level score:(int)score levelStatsFlag:(int)levelStatsFlag jonUnlockedAbilitiesFlag:(int)jonUnlockedAbilitiesFlag;

+ (int)getScorePushedOnline:(int)world level:(int)level;
+ (void)setScorePushedOnline:(int)world level:(int)level score:(int)score;

@end
