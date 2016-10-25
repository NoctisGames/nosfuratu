//
//  AppleSoundManager.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/23/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AppleSoundManager : NSObject

- (void)loadSound:(NSString *)path withNumCopies:(int)numCopies;

- (void)playSound:(int)rawResourceId volume:(float)volume isLooping:(bool)isLooping;

- (void)stopSound:(int)rawResourceId;

- (void)loadMusic:(NSString *)path;

- (void)playMusic:(float)volume isLooping:(bool)isLooping;

- (void)setMusicVolume:(float)volume;

- (void)resumeMusic;

- (void)pauseMusic;

@end
