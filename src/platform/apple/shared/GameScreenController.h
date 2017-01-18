//
//  GameScreenController.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/14/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "SaveData.h"

// C++
#include "MacOpenGLGameScreen.h"
#include "GameConstants.h"
#include "GameScreenLevelEditor.h"
#include "GameScreenWorldMap.h"
#include "Game.h"

typedef void (^DisplayMessageBlock)(NSString *message);
typedef NSString * (^GetLevelFilePath)(NSString *levelFileName);
typedef void (^HandleInterstitialAd)();

@interface GameScreenController : NSObject

- (instancetype)initWithGameScreen:(GameScreen *)gameScreen getLevelFilePath:(GetLevelFilePath)getLevelFilePath displayMessageBlock:(DisplayMessageBlock)displayMessageBlock andHandleInterstitialAd:(HandleInterstitialAd)handleInterstitialAd;

- (void)update:(float)deltaTime;

- (void)present;

- (void)resume;

- (void)pause;

@end
