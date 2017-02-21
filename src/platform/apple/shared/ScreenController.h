//
//  ScreenController.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/14/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#import <Foundation/Foundation.h>

// C++
#include "MainScreen.h"

typedef void (^HandleInterstitialAd)();

@interface ScreenController : NSObject

- (instancetype)initWithScreen:(MainScreen *)screen andInterstitialAdHandler:(HandleInterstitialAd)handleInterstitialAd;

- (void)update:(float)deltaTime;

- (void)present;

- (void)resume;

- (void)pause;

@end
