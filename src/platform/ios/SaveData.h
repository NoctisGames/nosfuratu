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

+ (int)getLevelStats:(int)world level:(int)level;

+ (void)setLevelComplete:(int)world level:(int)level goldenCarrotsFlag:(int)gcf;

@end
