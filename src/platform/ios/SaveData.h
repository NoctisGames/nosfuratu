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

+ (int)getJonUnlockedAbilitiesFlag;
+ (void)setJonUnlockedAbilitiesFlag:(int)jonUnlockedAbilitiesFlag;

+ (int)getJonViewedCutscenesFlag;
+ (void)setJonViewedCutscenesFlag:(int)jonViewedCutscenesFlag;

+ (int)getLevelScore:(int)world level:(int)level;

+ (int)getLevelStatsFlag:(int)world level:(int)level;

+ (void)setLevelComplete:(int)world level:(int)level score:(int)score levelStatsFlag:(int)levelStatsFlag;

+ (int)getScorePushedOnline:(int)world level:(int)level;
+ (void)setScorePushedOnline:(int)world level:(int)level score:(int)score;

@end
